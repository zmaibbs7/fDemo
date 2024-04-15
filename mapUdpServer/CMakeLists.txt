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

# 设置 Protobuf 源码路径
set(Protobuf_INCLUDE_DIR "C:/Users/zmaib/Project/github/fDemo/mapUdpServer/modules/protobuf/src")
# 设置 Protobuf 库文件路径（假设库文件在你的构建目录下的某个位置）
set(Protobuf_LIBRARY "C:/Users/zmaib/Project/github/fDemo/mapUdpServer/build/modules/protobuf/Release/libprotobuf.lib")
# 将 Protobuf 的库目录添加到链接目录
link_directories("C:/Users/zmaib/Project/github/fDemo/mapUdpServer/build/modules/protobuf")
# 使用 find_package
# 找到 Protobuf
find_package(Protobuf 3 REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

# 设置输出路径
set(MESSAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/hxUdpProto)
file(MAKE_DIRECTORY ${MESSAGE_DIR})

# 设置 protoc 的搜索路径和获取需要编译的 proto 文件
set(PROTO_SOURCE_DIR "C:/Users/zmaib/Project/github/fDemo/mapUdpServer/hxUdpProto")
file(GLOB_RECURSE MSG_PROTOS "${PROTO_SOURCE_DIR}/*.proto")

set(MESSAGE_SRC "")
set(MESSAGE_HDRS "")
set(PROTOBUF_PROTOC_EXECUTABLE C:/Users/zmaib/Project/github/fDemo/mapUdpServer/build/modules/protobuf/Release/protoc.exe)

foreach(msg ${MSG_PROTOS})
    get_filename_component(FIL_WE ${msg} NAME_WE)
    
    list(APPEND MESSAGE_SRC "${MESSAGE_DIR}/${FIL_WE}.pb.cc")
    list(APPEND MESSAGE_HDRS "${MESSAGE_DIR}/${FIL_WE}.pb.h")

    # 使用自定义命令
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

# 设置文件属性为 GENERATED
set_source_files_properties(${MESSAGE_SRC} ${MESSAGE_HDRS} PROPERTIES GENERATED TRUE)

# 添加自定义target
add_custom_target(generate_message ALL
                  DEPENDS ${MESSAGE_SRC} ${MESSAGE_HDRS}
                  COMMENT "generate message target"
                  VERBATIM
                  )
                  
add_executable(mapUdpServer
  main.cpp
  hxMapUdpServer.h
  hxMapUdpServer.cpp
  ${MESSAGE_SRC} ${MESSAGE_HDRS}
)
message("header.pb = ${PROTO_SRCS}")
message("PROTOBUF_PROTOC_EXECUTABLE = ${PROTOBUF_PROTOC_EXECUTABLE}")

target_link_libraries(mapUdpServer Qt${QT_VERSION_MAJOR}::Core hv_static protobuf::libprotobuf-lite)

include(GNUInstallDirs)
install(TARGETS mapUdpServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

