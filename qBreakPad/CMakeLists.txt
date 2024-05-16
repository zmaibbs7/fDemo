cmake_minimum_required(VERSION 3.14)

project(qBreakpadTest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core NetWork)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core NetWork)

add_subdirectory(modules)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/modules")

# 查找当前目录下所有的.cpp文件
file(GLOB_RECURSE PROJECT_SOURCES "src/*")

# 添加可执行文件
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${qBreakpad})
# 链接静态库
target_link_libraries(${PROJECT_NAME} PRIVATE qBreakpad)
