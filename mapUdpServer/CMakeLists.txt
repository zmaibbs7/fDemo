cmake_minimum_required(VERSION 3.14)

project(mapUdpServer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_subdirectory(modules)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/modules")

# set Protobuf
set(Protobuf_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/modules/protobuf/src")
set(Protobuf_LIBRARY "${CMAKE_BINARY_DIR}/modules/protobuf/Release/libprotobuf.lib")
link_directories("${CMAKE_BINARY_DIR}/modules/protobuf")

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS "protoMessage/header.proto")

add_executable(mapUdpServer
  main.cpp
  hxMapServer.h
  hxMapServer.cpp
  ${PROTO_SRCS} ${PROTO_HDRS})

message("header.pb = ${PROTO_SRCS}")
message("PROTO_HDRS = ${PROTO_HDRS}")
message("PROTOBUF_PROTOC_EXECUTABLE = ${PROTOBUF_PROTOC_EXECUTABLE}")
message("CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
message("CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")

target_link_libraries(mapUdpServer Qt${QT_VERSION_MAJOR}::Core hv_static protobuf::libprotobuf-lite)

include(GNUInstallDirs)
install(TARGETS mapUdpServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

