cmake_minimum_required(VERSION 3.14)

project(qBreakpadTest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    # 设置生成 PDB 文件的编译器标志，对于所有配置类型
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")

    # 确保 Release 模式下不会因生成 PDB 而优化掉调试信息
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")

    # 设置链接器标志以生成 PDB 文件，对于 Release 配置
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO}")
endif()

set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)

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

# dump_syms 
set(SYM_PATH ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(SYM_OUTPUT_FILE "${SYM_PATH}/${PROJECT_NAME}.sym")
message("dump_syms = ${CMAKE_BINARY_DIR}/modules/qBreakpad/${CMAKE_BUILD_TYPE}")

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Installing dump_syms.exe and generating symbol file"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/modules/qBreakpad/${CMAKE_BUILD_TYPE}/dump_syms.exe ${SYM_PATH}/dump_syms.exe
    COMMAND ${SYM_PATH}/dump_syms.exe ${SYM_PATH}/${PROJECT_NAME}.pdb > ${SYM_OUTPUT_FILE}
    COMMENT "Generating symbol file: ${SYM_OUTPUT_FILE}"
)