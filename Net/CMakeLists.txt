cmake_minimum_required(VERSION 3.14)

project(test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(modules)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/modules")

# 查找当前目录下所有的.cpp文件
file(GLOB APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

# 为每个.cpp文件创建一个可执行文件
foreach(TEST_SOURCE ${APP_SOURCES})
    # 获取不带扩展名的文件名
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    # 添加可执行文件
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_include_directories(${TEST_NAME} PRIVATE ${hv_static})
    # 链接静态库
    target_link_libraries(${TEST_NAME} PRIVATE hv_static)
endforeach()
