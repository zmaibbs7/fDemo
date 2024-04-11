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

add_executable(mapUdpServer
  main.cpp
  hxMapUdpServer.h
  hxMapUdpServer.cpp
  hxUdpProto/header.pb.h
  hxUdpProto/header.pb.cc
)

target_link_libraries(mapUdpServer Qt${QT_VERSION_MAJOR}::Core hv_static protobuf::libprotobuf)

include(GNUInstallDirs)
install(TARGETS mapUdpServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)