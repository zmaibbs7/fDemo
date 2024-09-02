cmake_minimum_required(VERSION 3.14)

project(test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(modules)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/modules")

# 设置项目源文件列表
file(GLOB_RECURSE PROJECT_SOURCES
     "src/*.cpp" "src/*.h"
    )

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ghc_filesystem)