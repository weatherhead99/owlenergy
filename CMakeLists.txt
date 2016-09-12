cmake_minimum_required(VERSION 3.2)
project(owlenergy)
set(CMAKE_CXX_STANDARD 11)

find_library(libusb usb-1.0 REQUIRED)
find_path(libusb_include NAMES libusb.h PATH_SUFFIXES libusb-1.0)

message("libusb library: ${libusb}")
message("libusb include: ${libusb_include}")

include_directories(${libusb_include})

add_executable(owl main.cpp owl.cpp)
target_link_libraries(owl ${libusb})