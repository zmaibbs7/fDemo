cmake_minimum_required(VERSION 3.14)

project(test VERSION 0.1 LANGUAGES CXX)

# 查找src目录下所有目录

# 设置变量，指向你的源代码目录
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# 使用GLOB指令查找所有子目录
file(GLOB CHILDREN RELATIVE ${SOURCE_DIR} ${SOURCE_DIR}/*)
foreach(child ${CHILDREN})
    if(IS_DIRECTORY ${SOURCE_DIR}/${child})
        add_subdirectory(src/${child})
    endif()
endforeach()
