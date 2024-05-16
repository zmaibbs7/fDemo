if(NOT TARGET libcurl)
    # 设置curl库的构建选项
    set(BUILD_CURL_EXE OFF CACHE BOOL "Set to ON to build curl executable.")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
    set(BUILD_STATIC_LIBS ON CACHE BOOL "Build static libraries")
    set(CURL_STATICLIB ON CACHE BOOL "Build curl executable with static libcurl")

    # Module subdirectory
    add_subdirectory("curl-8.7.1")

endif()

