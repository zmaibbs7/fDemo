// #include <chrono>
// #include <iomanip>
// #include <iostream>
// #include <string>

// #if defined(__cplusplus) && __cplusplus >= 201703L && defined(__has_include)
// #if __has_include(<filesystem>)
// #define GHC_USE_STD_FS
// #include <filesystem>
// namespace fs = std::filesystem;
// #endif
// #endif
// #ifndef GHC_USE_STD_FS
// #include <ghc/filesystem.hpp>
// namespace fs = ghc::filesystem;
// #endif

// template <typename TP>
// std::time_t to_time_t(TP tp)
// {
//     // Based on trick from: Nico Josuttis, C++17 - The Complete Guide
//     std::chrono::system_clock::duration dt = std::chrono::duration_cast<std::chrono::system_clock::duration>(tp - TP::clock::now());
//     return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() + dt);
// }

// static std::string perm_to_str(fs::perms prms)
// {
//     std::string result;
//     result.reserve(9);
//     for (int i = 0; i < 9; ++i) {
//         result = ((static_cast<int>(prms) & (1 << i)) ? "xwrxwrxwr"[i] : '-') + result;
//     }
//     return result;
// }

#include <iostream>
#include <ghc/filesystem.hpp>  // 使用 ghc::filesystem 代替 std::filesystem
#include <vector>

namespace fs = ghc::filesystem;

std::vector<fs::path> getSubdirectories(const fs::path& directory) {
    std::vector<fs::path> subdirectories;

    // 检查目录是否存在且是一个目录
    if (fs::exists(directory) && fs::is_directory(directory)) {
        for (const auto& entry : fs::directory_iterator(directory)) {
            // 检查当前条目是否是一个目录
            if (fs::is_directory(entry.path())) {
                subdirectories.push_back(entry.path());
            }
        }
    } else {
        std::cerr << "Directory does not exist or is not a directory: " << directory << std::endl;
    }

    return subdirectories;
}

int main() {
    fs::path directory = R"(C:\Users\zmaib\Project\dreame\G2402\map)";  // 指定要查找的目录
    std::vector<fs::path> subdirectories = getSubdirectories(directory);

    std::cout << "Subdirectories in " << directory << ":\n";
    for (const auto& subdirectory : subdirectories) {
        std::cout << subdirectory << std::endl;
    }

    return 0;
}

