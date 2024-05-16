cmake_minimum_required(VERSION 3.5)

project(qBreakpad)
# 定义 UNICODE 宏
add_definitions(-D_UNICODE -DUNICODE)
# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_language(ASM)
# 找到Qt5，确保已经设置了Qt5_DIR或者环境变量
find_package(Qt5 COMPONENTS Core Network REQUIRED)

# 设置Breakpad的路径
set(BREAKPAD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_party/breakpad/src")

# 定义预处理器变量
add_definitions(-DCALIPER)

# 设置使用静态库
set(BUILD_SHARED_LIBS OFF)

# 包括Breakpad和当前项目的头文件目录
include_directories(${BREAKPAD_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

# 设置源文件列表
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/handler/QBreakpadHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/handler/QBreakpadHttpUploader.cpp
)

# 根据平台特定的源文件
if(APPLE)
    list(APPEND SOURCES
        ${BREAKPAD_PATH}/client/mac/crash_generation/crash_generation_client.cc
        ${BREAKPAD_PATH}/client/mac/handler/breakpad_nlist_64.cc
        ${BREAKPAD_PATH}/client/mac/handler/dynamic_images.cc
        ${BREAKPAD_PATH}/client/mac/handler/exception_handler.cc
        ${BREAKPAD_PATH}/client/mac/handler/minidump_generator.cc
        ${BREAKPAD_PATH}/common/mac/MachIPC.mm
        ${BREAKPAD_PATH}/common/mac/bootstrap_compat.cc
        ${BREAKPAD_PATH}/common/mac/file_id.cc
        ${BREAKPAD_PATH}/common/mac/macho_id.cc
        ${BREAKPAD_PATH}/common/mac/macho_utilities.cc
        ${BREAKPAD_PATH}/common/mac/macho_walker.cc
        ${BREAKPAD_PATH}/common/mac/string_utilities.cc
    )
elseif(UNIX AND NOT APPLE)
    list(APPEND SOURCES
        ${BREAKPAD_PATH}/client/linux/crash_generation/crash_generation_client.cc
        ${BREAKPAD_PATH}/client/linux/dump_writer_common/thread_info.cc
        ${BREAKPAD_PATH}/client/linux/dump_writer_common/ucontext_reader.cc
        ${BREAKPAD_PATH}/client/linux/handler/exception_handler.cc
        ${BREAKPAD_PATH}/client/linux/handler/minidump_descriptor.cc
        ${BREAKPAD_PATH}/client/linux/log/log.cc
        ${BREAKPAD_PATH}/client/linux/microdump_writer/microdump_writer.cc
        ${BREAKPAD_PATH}/client/linux/minidump_writer/linux_core_dumper.cc
        ${BREAKPAD_PATH}/client/linux/minidump_writer/linux_dumper.cc
        ${BREAKPAD_PATH}/client/linux/minidump_writer/linux_ptrace_dumper.cc
        ${BREAKPAD_PATH}/client/linux/minidump_writer/minidump_writer.cc
        ${BREAKPAD_PATH}/client/linux/minidump_writer/pe_file.cc
        ${BREAKPAD_PATH}/common/linux/breakpad_getcontext.S
        ${BREAKPAD_PATH}/common/linux/elfutils.cc
        ${BREAKPAD_PATH}/common/linux/file_id.cc
        ${BREAKPAD_PATH}/common/linux/guid_creator.cc
        ${BREAKPAD_PATH}/common/linux/linux_libc_support.cc
        ${BREAKPAD_PATH}/common/linux/memory_mapped_file.cc
        ${BREAKPAD_PATH}/common/linux/safe_readlink.cc
        # Unix/Linux 通用源文件
        ${BREAKPAD_PATH}/client/minidump_file_writer.cc
        ${BREAKPAD_PATH}/common/convert_UTF.cc
        ${BREAKPAD_PATH}/common/md5.cc
        ${BREAKPAD_PATH}/common/string_conversion.cc
    )
elseif(WIN32)
    list(APPEND SOURCES
        ${BREAKPAD_PATH}/client/windows/crash_generation/crash_generation_client.cc
        ${BREAKPAD_PATH}/client/windows/handler/exception_handler.cc
        ${BREAKPAD_PATH}/common/windows/guid_string.cc
    )
endif()

# Set source files properties for assembly file
set_source_files_properties(
    "${BREAKPAD_PATH}/common/linux/breakpad_getcontext.S"
    PROPERTIES LANGUAGE ASM
)
# 添加库目标
add_library(qBreakpad ${SOURCES})

# 设置头文件列表
set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/handler/singletone/call_once.h
    ${CMAKE_CURRENT_SOURCE_DIR}/handler/singletone/singleton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/handler/QBreakpadHandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/handler/QBreakpadHttpUploader.h
)

# 包括目录
target_include_directories(qBreakpad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/handler)

# 链接Qt5库
target_link_libraries(qBreakpad Qt5::Core Qt5::Network)
