// #ifndef GHCFILESYSTEM_WRAPPER_H
// #define GHCFILESYSTEM_WRAPPER_H

// #include <ghc/filesystem.hpp>
// #include <string>
// #include <vector>

// namespace fs {

//     using path = ghc::filesystem::path;
//     using directory_entry = ghc::filesystem::directory_entry;
//     using directory_iterator = ghc::filesystem::directory_iterator;
//     using recursive_directory_iterator = ghc::filesystem::recursive_directory_iterator;
//     using file_status = ghc::filesystem::file_status;
//     using file_type = ghc::filesystem::file_type;
//     using perms = ghc::filesystem::perms;

//     inline bool exists(const path& p);

//     inline bool is_directory(const path& p);

//     inline bool is_regular_file(const path& p);

//     inline std::uintmax_t file_size(const path& p);

//     inline void create_directory(const path& p);

//     inline void remove(const path& p);

//     inline void remove_all(const path& p);

//     inline std::vector<path> list_directory(const path& p);

//     inline void copy(const path& from, const path& to);

//     inline void rename(const path& from, const path& to);

//     // 更多功能可以按需添加
// }

// #endif // GHCFILESYSTEM_WRAPPER_H
