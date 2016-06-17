cmake_minimum_required (VERSION 2.6)
project(openrov-experimental-protobuf)

set(CMAKE_CXX_FLAGS "-std=c++11")

#include the nanopb headers and source
set(NANOPB_DIR libs/nanopb)
include_directories(${NANOPB_DIR})

#Get the nanopb sources
file(GLOB NANOPB_SRCS ${NANOPB_DIR}/*.c)

set(SRCS src/main.cpp
    src/NCommManager.cpp
    src/PositionSensor.pb.cpp    
    src/CPositionSensor.cpp
    ${NANOPB_SRCS})

add_executable(${PROJECT_NAME} ${SRCS})

