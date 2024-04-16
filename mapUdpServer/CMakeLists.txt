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

# 设置 Protobuf 源代码目录
set(Protobuf_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/modules/protobuf/src")

# 设置 Protobuf 库文件的路径
set(Protobuf_LIBRARY "${CMAKE_BINARY_DIR}/modules/protobuf/Release/libprotobuf.lib")

# 添加 Protobuf 库目录到链接目录，确保链接器能够找到 Protobuf 库
link_directories("${CMAKE_BINARY_DIR}/modules/protobuf")

# 寻找 Protobuf 包
find_package(Protobuf 3 REQUIRED)

# 添加 Protobuf 头文件目录到包含目录
include_directories(${Protobuf_INCLUDE_DIRS})

# 定义用于存放生成的 Protobuf 消息类源代码和头文件的目录
set(MESSAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/protoMessage")
file(MAKE_DIRECTORY ${MESSAGE_DIR})

# 设置 Protobuf 文件的搜索路径，以及获取需要编译的 proto 文件的列表
set(PROTO_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/protoMessage")
file(GLOB_RECURSE MSG_PROTOS "${PROTO_SOURCE_DIR}/*.proto")

# 初始化用于存储生成的 Protobuf 源文件和头文件路径的变量
set(MESSAGE_SRC "")
set(MESSAGE_HDRS "")

# 设置 protoc 的可执行文件路径，确保使用与库匹配的版本
set(PROTOBUF_PROTOC_EXECUTABLE "${CMAKE_BINARY_DIR}/modules/protobuf/Release/protoc.exe")

# 遍历所有的 Proto 文件，为每个文件生成相应的 C++ 源代码和头文件
foreach(msg ${MSG_PROTOS})
    get_filename_component(FIL_WE ${msg} NAME_WE)
    
    list(APPEND MESSAGE_SRC "${MESSAGE_DIR}/${FIL_WE}.pb.cc")
    list(APPEND MESSAGE_HDRS "${MESSAGE_DIR}/${FIL_WE}.pb.h")

    # 添加自定义命令来生成 Proto 文件的 C++ 源代码和头文件
    add_custom_command(
        OUTPUT "${MESSAGE_DIR}/${FIL_WE}.pb.cc"
               "${MESSAGE_DIR}/${FIL_WE}.pb.h"
        COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
        ARGS --cpp_out=${MESSAGE_DIR}
             --proto_path=${PROTO_SOURCE_DIR}
             ${msg}
        DEPENDS ${msg}
        COMMENT "Running C++ protocol buffer compiler on ${msg}"
        VERBATIM
    )
endforeach()

# 标记生成的源文件和头文件为由外部工具生成，确保 CMake 处理它们时能正确理解
set_source_files_properties(${MESSAGE_SRC} ${MESSAGE_HDRS} PROPERTIES GENERATED TRUE)

# 添加一个自定义目标来确保在每次构建前都会生成最新的消息文件
add_custom_target(generate_message ALL
                  DEPENDS ${MESSAGE_SRC} ${MESSAGE_HDRS}
                  COMMENT "Generating Protobuf message sources and headers"
                  VERBATIM
                  )

add_executable(mapUdpServer
  main.cpp
  hxMapServer.h
  hxMapServer.cpp
  ${MESSAGE_SRC} ${MESSAGE_HDRS}
)
message("header.pb = ${PROTO_SRCS}")
message("PROTOBUF_PROTOC_EXECUTABLE = ${PROTOBUF_PROTOC_EXECUTABLE}")
message("CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
message("CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")

target_link_libraries(mapUdpServer Qt${QT_VERSION_MAJOR}::Core hv_static protobuf::libprotobuf-lite)

include(GNUInstallDirs)
install(TARGETS mapUdpServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

