cmake_minimum_required (VERSION 2.6)
project(openrov-experimental-protobuf)

set(CMAKE_CXX_FLAGS "-std=c++11")

#NanoPB Library
set(NANOPB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb)
MESSAGE(STATUS "Looking in " ${NANOPB_INCLUDE_DIR} " for nanopb headers")
include_directories(${NANOPB_INCLUDE_DIR})
set(NANOPB_SOURCES ${NANOPB_INCLUDE_DIR}/pb_common.c
                   ${NANOPB_INCLUDE_DIR}/pb_decode.c
                   ${NANOPB_INCLUDE_DIR}/pb_encode.c)                   


#Common
set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/common)
MESSAGE(STATUS "Looking in " ${COMMON_INCLUDE_DIR} " for common headers")
include_directories(${COMMON_INCLUDE_DIR})
set(COMMON_SOURCES ${COMMON_INCLUDE_DIR}/NCommManager.cpp)

#Plugins
set(PLUGIN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins)
MESSAGE(STATUS "Looking in " ${PLUGIN_INCLUDE_DIR} " for plugin headers")
include_directories(${PLUGIN_INCLUDE_DIR})

file(GLOB SUB_DIRS ${PLUGIN_INCLUDE_DIR}/*)

FOREACH(dir in ${SUB_DIRS})
  file(GLOB SOURCE_FILE ${dir}/*cpp)
  list(APPEND PLUGIN_SOURCES ${SOURCE_FILE})
ENDFOREACH()


#Generated protofiles
set(GEN_PROTO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/proto)
MESSAGE(STATUS "Looking in " ${GEN_PROTO_INCLUDE_DIR} " for compiled proto headers")
include_directories(${GEN_PROTO_INCLUDE_DIR})
file(GLOB GEN_PROTO_SOURCES ${GEN_PROTO_INCLUDE_DIR}/*.cpp)


set(SOURCES src/main.cpp
            ${NANOPB_SOURCES}
            ${COMMON_SOURCES}
            ${GEN_PROTO_SOURCES}
            ${PLUGIN_SOURCES})

add_executable(${PROJECT_NAME} ${SOURCES})

