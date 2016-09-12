#include <iostream>

using std::cout;
using std::endl;

#include <memory>

#include <libusb.h>
#include <vector>


#include "owl.h"

int main(int argc, char** argv)
{
    cout << "init" << endl;
    
    owlEnergy owl;
    
    auto dat = owl.bulkread();
    
    cout << "read: " <<dat << endl;
    
  
};