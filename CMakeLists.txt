cmake_minimum_required(VERSION 3.14)

project(fDemo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Modules
add_subdirectory("modules")
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/modules")

file(GLOB_RECURSE PROJECT_SOURCES
     "include/*.h" "src/*.cpp")

add_executable(fDemo ${PROJECT_SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE -DCMakePath="${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        "${CMAKE_CURRENT_SOURCE_DIR}/include"

    )
target_link_libraries(${PROJECT_NAME}
    # PRIVATE
    )

# 安装设置
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)

# utf-8
if(MSVC)
   target_compile_options(${PROJECT_NAME} PRIVATE "/execution-charset:utf-8" "/source-charset:utf-8")
   # Disable warning 4819
   target_compile_options(${PROJECT_NAME} PRIVATE "/wd4819")
   target_compile_options(${PROJECT_NAME} PRIVATE "/wd4996")
endif()
