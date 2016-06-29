cmake_minimum_required (VERSION 2.6)
project(openrov-experimental-protobuf)

set(CMAKE_CXX_FLAGS "-std=c++11")

#include the nanopb headers and source
set(NANOPB_DIR libs/nanopb)
include_directories(${NANOPB_DIR})

#Get the nanopb sources
file(GLOB NANOPB_SRCS ${NANOPB_DIR}/*.c)

#Get the plugin sources
file(GLOB PLUGIN_SRCS src/plugins/*cpp)

#Get the generated proto sources
file(GLOB PROTO_SRCS src/proto/*.cpp)
include_directories(src/proto)

#Common sources
include_directories(src/common)
set(COMMON_SRCS src/common/NCommManager.cpp)
set(SRCS src/main.cpp
     ${PLUGIN_SRCS}
     ${NANOPB_SRCS}
     ${PROTO_SRCS}
     ${COMMON_SRCS}
)

add_executable(${PROJECT_NAME} ${SRCS})

