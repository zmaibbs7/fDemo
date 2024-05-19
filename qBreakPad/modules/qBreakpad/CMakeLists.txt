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

# 查找DIA SDK
set(CMAKE_GENERATOR_PLATFORM "x64" )
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(FindDiaSDK)
include(FindDbgHelp)


include_directories("${DIASDK_INCLUDE_DIR}")
message("Congraute Find DIASDK_INCLUDE_DIR = ${DIASDK_INCLUDE_DIR}")

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

# 添加选项来控制是否生成dump_syms minidump_stackwalk可执行文件
option(BUILD_DUMP_SYMS "Build dump_syms tool" ON)
option(BUILD_MINIDUMP_STACKWALK "Build minidump_stackwalk tool" ON)

if(BUILD_DUMP_SYMS)
    if(WIN32)
        # 确保链接到 diaguids.lib 和必要的系统库
        set(WINDOWS_LIBS "Ole32.lib" "Uuid.lib" "${DIA_SDK_LIB}" "Dbghelp.lib" "Imagehlp.lib")

        add_executable(dump_syms
            ${BREAKPAD_PATH}/tools/windows/dump_syms/dump_syms.cc
            ${BREAKPAD_PATH}/common/windows/dia_util.cc
            ${BREAKPAD_PATH}/common/windows/guid_string.cc
            ${BREAKPAD_PATH}/common/windows/omap.cc
            ${BREAKPAD_PATH}/common/windows/pdb_source_line_writer.cc
            ${BREAKPAD_PATH}/common/windows/pe_source_line_writer.cc
            ${BREAKPAD_PATH}/common/windows/pe_util.cc
            ${BREAKPAD_PATH}/common/windows/string_utils.cc
        )
        target_include_directories(dump_syms PRIVATE ${BREAKPAD_PATH})
        target_link_libraries(dump_syms qBreakpad ${DBGHELP_LIBRARY} ${DIASDK_GUIDS_LIBRARY} ${DIA_SDK_LIB} ${WINDOWS_LIBS})

        # 注册msdia*.dll
        message("regsvr32 -> ${VSWHERE_LATEST}\\DIA SDK\\bin\\amd64\\msdia140.dll")
        add_custom_command(TARGET dump_syms POST_BUILD
            COMMAND regsvr32 /s "${VSWHERE_LATEST}\\DIA SDK\\bin\\amd64\\msdia140.dll"
            COMMAND regsvr32 /s "${VSWHERE_LATEST}\\DIA SDK\\bin\\msdia140.dll"
            COMMENT "Registering msdia140.dll"
        )
    endif()
endif()

if(BUILD_MINIDUMP_STACKWALK)
    if(WIN32)
        # add_executable(minidump_stackwalk
        #     ${BREAKPAD_PATH}/getopt.c
        #     ${BREAKPAD_PATH}/common/windows/dia_util.cc
        #     ${BREAKPAD_PATH}/common/windows/guid_string.cc
        #     ${BREAKPAD_PATH}/common/windows/omap.cc
        #     ${BREAKPAD_PATH}/common/windows/pdb_source_line_writer.cc
        #     ${BREAKPAD_PATH}/common/windows/pe_source_line_writer.cc
        #     ${BREAKPAD_PATH}/common/windows/pe_util.cc
        #     ${BREAKPAD_PATH}/common/windows/string_utils.cc
        #     ${BREAKPAD_PATH}/common/path_helper.cc
        #     ${BREAKPAD_PATH}/processor/basic_code_modules.cc
        #     ${BREAKPAD_PATH}/processor/basic_source_line_resolver.cc
        #     ${BREAKPAD_PATH}/processor/call_stack.cc
        #     ${BREAKPAD_PATH}/processor/cfi_frame_info.cc
        #     # ${BREAKPAD_PATH}/processor/convert_old_arm64_context.cc
        #     ${BREAKPAD_PATH}/processor/disassembler_objdump.cc
        #     ${BREAKPAD_PATH}/processor/disassembler_x86.cc
        #     ${BREAKPAD_PATH}/processor/dump_context.cc
        #     ${BREAKPAD_PATH}/processor/dump_object.cc
        #     ${BREAKPAD_PATH}/processor/exploitability.cc
        #     ${BREAKPAD_PATH}/processor/exploitability_linux.cc
        #     ${BREAKPAD_PATH}/processor/exploitability_win.cc
        #     ${BREAKPAD_PATH}/processor/fast_source_line_resolver.cc
        #     ${BREAKPAD_PATH}/processor/logging.cc
        #     ${BREAKPAD_PATH}/processor/microdump.cc
        #     ${BREAKPAD_PATH}/processor/microdump_processor.cc
        #     ${BREAKPAD_PATH}/processor/microdump_stackwalk.cc
        #     ${BREAKPAD_PATH}/processor/minidump.cc
        #     ${BREAKPAD_PATH}/processor/minidump_dump.cc
        #     ${BREAKPAD_PATH}/processor/minidump_processor.cc
        #     ${BREAKPAD_PATH}/processor/minidump_stackwalk.cc
        #     ${BREAKPAD_PATH}/processor/module_comparer.cc
        #     ${BREAKPAD_PATH}/processor/module_serializer.cc
        #     ${BREAKPAD_PATH}/processor/pathname_stripper.cc
        #     ${BREAKPAD_PATH}/processor/process_state.cc
        #     ${BREAKPAD_PATH}/processor/proc_maps_linux.cc
        #     ${BREAKPAD_PATH}/processor/simple_symbol_supplier.cc
        #     ${BREAKPAD_PATH}/processor/source_line_resolver_base.cc
        #     ${BREAKPAD_PATH}/processor/stackwalker.cc
        #     ${BREAKPAD_PATH}/processor/stackwalker_address_list.cc
        #     ${BREAKPAD_PATH}/processor/stackwalker_amd64.cc
        #     ${BREAKPAD_PATH}/processor/stackwalker_x86.cc
        #     ${BREAKPAD_PATH}/processor/stackwalk_common.cc
        #     ${BREAKPAD_PATH}/processor/stack_frame_cpu.cc
        #     ${BREAKPAD_PATH}/processor/stack_frame_symbolizer.cc
        #     ${BREAKPAD_PATH}/processor/symbolic_constants_win.cc
        #     ${BREAKPAD_PATH}/processor/synth_minidump.cc
        #     ${BREAKPAD_PATH}/processor/tokenize.cc
        # )
        # target_include_directories(minidump_stackwalk PRIVATE ${BREAKPAD_PATH})
        # target_link_libraries(minidump_stackwalk qBreakpad ${DBGHELP_LIBRARY} ${DIASDK_GUIDS_LIBRARY} ${DIA_SDK_LIB} ${WINDOWS_LIBS})
    endif()
endif()
