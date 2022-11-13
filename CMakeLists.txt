cmake_minimum_required(VERSION 3.5)
project(Communication)

set(INC_DIR /usr/local/include)
set(LINK_DIR /usr/local/lib)

set(CMAKE_CXX_STANDARD 14)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})
add_executable(Communication main.cpp Inter_Board_Com.cpp USB_IMU.cpp)
target_link_libraries(Communication libusb-1.0.so)
