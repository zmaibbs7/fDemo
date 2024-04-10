cmake_minimum_required(VERSION 3.16)

# 项目名称和版本
project(test VERSION 0.1 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置使用静态链接运行时库 (MT, MTd)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# 添加模块子目录
add_subdirectory(modules)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/modules")

# 添加可执行文件
add_executable(${PROJECT_NAME} header.pb.h main.cpp header.pb.cc )

# 链接Protobuf库 后面改成lite
target_link_libraries(${PROJECT_NAME} PRIVATE protobuf::libprotobuf)
