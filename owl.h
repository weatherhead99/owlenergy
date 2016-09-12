#pragma once

#include <string>

class libusb_context;
class libusb_device_handle;

class owlEnergy
{
public:
  owlEnergy();
  ~owlEnergy();
  
  std::string bulkread();
  
  

private:
    void respond_to_id_msg();
  
    libusb_context* _ctxt;
    libusb_device_handle* _hdl;
};