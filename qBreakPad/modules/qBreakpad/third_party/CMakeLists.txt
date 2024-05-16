cmake_minimum_required(VERSION 3.5)
project(breakpad-qt)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置Breakpad路径
set(BREAKPAD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/breakpad/src")

# 包含目录
include_directories(${BREAKPAD_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/..)

# 设置源文件
set(SOURCES
    ${BREAKPAD_PATH}/client/minidump_file_writer.cc
    ${BREAKPAD_PATH}/common/convert_UTF.cc
    ${BREAKPAD_PATH}/common/md5.cc
    ${BREAKPAD_PATH}/common/string_conversion.cc
)

if(UNIX AND NOT APPLE)
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
        ${BREAKPAD_PATH}/common/linux/breakpad_getcontext.S
        ${BREAKPAD_PATH}/common/linux/elfutils.cc
        ${BREAKPAD_PATH}/common/linux/file_id.cc
        ${BREAKPAD_PATH}/common/linux/guid_creator.cc
        ${BREAKPAD_PATH}/common/linux/linux_libc_support.cc
        ${BREAKPAD_PATH}/common/linux/memory_mapped_file.cc
        ${BREAKPAD_PATH}/common/linux/safe_readlink.cc
    )
elseif(APPLE)
    set(OBJECTIVE_SOURCES
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
    set(SOURCES ${SOURCES} ${OBJECTIVE_SOURCES})
elseif(WIN32)
    list(APPEND SOURCES
        ${BREAKPAD_PATH}/client/windows/crash_generation/crash_generation_client.cc
        ${BREAKPAD_PATH}/client/windows/handler/exception_handler.cc
        ${BREAKPAD_PATH}/common/windows/guid_string.cc
    )
endif()

# 添加库目标
add_library(breakpad-qt ${SOURCES})

# 为Mac OS X设置Objective-C++编译选项
if(APPLE)
    set_source_files_properties(${OBJECTIVE_SOURCES} PROPERTIES COMPILE_FLAGS "-x objective-c++")
endif()
