#include "owl.h"

#include <libusb.h>

#include <stdexcept>
#include <string>
#include <vector>

#define OWL_VID 0x0fde
#define OWL_PID 0xca05

#define OWL_EP_IN 0x82
#define OWL_EP_OUT 0x01

#define OWL_BAUD_RATE 250000

#define CP210X_IFC_ENABLE 0x00
#define CP210X_SET_BAUDRATE 0x1E

#define CP210X_UART_ENABLE 1
#define CP210X_UART_DISABLE 0

#define INIT_TIMEOUT 500

#define BULK_TRANSFER_LEN 10000

#include <iostream>
using std::cout;
using std::endl;


const std::string id_msg = "IDTCMV001";


owlEnergy::owlEnergy()
{
  {
    auto ret = libusb_init(&_ctxt);
    if( ret <0)
    {
    throw std::runtime_error(std::string("libusb error: ") +  libusb_strerror((libusb_error) ret));
    }
  }
    
  _hdl = libusb_open_device_with_vid_pid(_ctxt, OWL_VID, OWL_PID);
  
  if(_hdl == nullptr)
  {
   throw std::runtime_error("couldn't open device"); 
  }
  
  if(libusb_kernel_driver_active(_hdl,0))
  {
    cout << "detaching kernel driver" << endl;
    libusb_detach_kernel_driver(_hdl,0);
  }
  
  {
    auto ret = libusb_claim_interface(_hdl,0);
    if(ret < 0)
    {
      throw std::runtime_error( std::string("couldn't claim interface") + libusb_strerror((libusb_error) ret));
    }
  }
  
  
  //init transfers
  //TODO: request type
  uint8_t reqtype = LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT;
  
  int baudrate = OWL_BAUD_RATE;
  
  int ret;
  
  if( (ret = libusb_control_transfer(_hdl, reqtype, CP210X_IFC_ENABLE, CP210X_UART_ENABLE, 0 , nullptr, 0, INIT_TIMEOUT ) )< 0)
  {
    throw std::runtime_error(std::string("libusb error: ") + libusb_strerror( (libusb_error) ret));
  }
    
  if( (ret = libusb_control_transfer(_hdl, reqtype, CP210X_SET_BAUDRATE, 0, 0, reinterpret_cast<unsigned char*>(&baudrate), sizeof(baudrate), INIT_TIMEOUT)) < 0) 
  {
   throw std::runtime_error(std::string("libusb error: ") + libusb_strerror( (libusb_error) ret)); 
  }
  
  if ( (ret = libusb_control_transfer(_hdl, reqtype, CP210X_IFC_ENABLE, CP210X_UART_DISABLE, 0, nullptr, 0, INIT_TIMEOUT)) < 0)
  {
    throw std::runtime_error(std::string("libusb error: " ) + libusb_strerror( (libusb_error) ret));
  }
  
  
  
}



owlEnergy::~owlEnergy()
{
  libusb_close(_hdl);
  libusb_exit(_ctxt);
}



std::string owlEnergy::bulkread()
{
  std::vector<unsigned char> buffer;
  buffer.resize(BULK_TRANSFER_LEN);
  
  int len = BULK_TRANSFER_LEN;
  
  auto ret = libusb_bulk_transfer(_hdl,OWL_EP_IN, buffer.data(), buffer.size(), &len, 10000);
  if(ret < 0)
  {
   throw std::runtime_error(std::string("libusb error: " ) + libusb_strerror( (libusb_error) ret)); 
    
  }
  
  
  return std::string(buffer.begin(), buffer.end());
  
}

void owlEnergy::respond_to_id_msg()
{
  unsigned char reply = 'Z';
  int len = 1;
  libusb_bulk_transfer(_hdl,OWL_EP_OUT,&reply, sizeof(reply), &len, 1000);
  

}


