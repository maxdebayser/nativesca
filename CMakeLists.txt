cmake_minimum_required(VERSION 2.8)

project(nativesca)

#SET(CMAKE_CXX_COMPILER "g++-4.7")
SET(CMAKE_CXX_COMPILER "clang++")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # Add c++11 functionality  
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti") # Add c++11 functionality  
#add_definitions(-DNO_RTTI)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)
find_package(TUSCANY_NATIVE REQUIRED)
find_package(SelfPortrait REQUIRED)

SET(IOC_INSTALL_DIR "${SCA_EXT_DIR}/ioc")

ENABLE_TESTING()
add_subdirectory(src)
