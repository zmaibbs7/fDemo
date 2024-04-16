cmake_minimum_required(VERSION 3.14)

project(mapUdpServer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(protobuf_BUILD_TESTS OFF CACHE BOOL "Build protobuf tests" FORCE)
set(protobuf_BUILD_PROTOC_BINARIES ON CACHE BOOL "Build protoc compiler" FORCE)
set(protobuf_BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(protobuf_WITH_ZLIB OFF CACHE BOOL "Build with zlib support" FORCE)

add_subdirectory(modules)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/modules")


set(Protobuf_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/modules/protobuf/src")
set(Protobuf_LIBRARY "${CMAKE_BINARY_DIR}/modules/protobuf/Release/libprotobuf.lib")
# set(PROTOC_EXECUTABLE "${CMAKE_BINARY_DIR}/modules/protobuf/Release/protoc")

find_package(Protobuf 3.21.4 REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

set(PROTOC_EXECUTABLE $<TARGET_FILE:protoc>)
set(PROTO_FILES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/protoMessage")
set(PROTO_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/protoMessage")

file(GLOB_RECURSE PROTO_FILES "${PROTO_FILES_DIR}/*.proto")

foreach(PROTO_FILE ${PROTO_FILES})
  get_filename_component(FILE_WE ${PROTO_FILE} NAME_WE)

  set(PROTO_SRC "${PROTO_OUTPUT_DIR}/${FILE_WE}.pb.cc")
  set(PROTO_HDR "${PROTO_OUTPUT_DIR}/${FILE_WE}.pb.h")

  add_custom_command(
    OUTPUT "${PROTO_SRC}" "${PROTO_HDR}"
    COMMAND ${PROTOC_EXECUTABLE}
    ARGS --cpp_out=${PROTO_OUTPUT_DIR} --proto_path=${PROTO_FILES_DIR} ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
    COMMENT "Running C++ protocol buffer compiler on ${PROTO_FILE}"
    VERBATIM
  )

  list(APPEND PROTO_SRCS "${PROTO_SRC}")
  list(APPEND PROTO_HDRS "${PROTO_HDR}")
endforeach()

add_custom_target(generate_protos ALL
  DEPENDS ${PROTO_SRCS} ${PROTO_HDRS}
  COMMENT "Generating Protobuf C++ sources and headers"
)

add_executable(mapUdpServer
  main.cpp
  hxMapServer.h
  hxMapServer.cpp
  ${PROTO_SRCS} ${PROTO_HDRS})


target_link_libraries(mapUdpServer Qt${QT_VERSION_MAJOR}::Core hv_static ${Protobuf_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS mapUdpServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

