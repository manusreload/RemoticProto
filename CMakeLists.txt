cmake_minimum_required(VERSION 3.5)
project(RemoticProto)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(REMOTIC_SOURCES Message.cpp Protocol.cpp CRC16.cpp VERSION001.cpp)
set(SOURCE_FILES test.cpp ${REMOTIC_SOURCES})
add_executable(RemoticProto ${SOURCE_FILES})