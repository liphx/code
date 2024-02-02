#ifndef LIPH_FILE_H_
#define LIPH_FILE_H_

#include <cstdio>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace liph {

/// read all from pathname, append to output
[[nodiscard]] bool read_file(const std::filesystem::path& pathname, std::string& output);

/// write all to pathname
[[nodiscard]] bool write_file(const std::filesystem::path& pathname, const std::string& str);

/// check path(file or directory) exists
[[nodiscard]] bool path_exists(const std::string& pathname);

/// list all files(including directories) in pathname recursively and sort by names
[[nodiscard]] std::vector<std::string> list_files(const std::string& pathname);

using file_ptr = std::unique_ptr<FILE, decltype([](FILE *f) {
    if (f) fclose(f);
})>;

inline file_ptr fopen(const char *filename, char const *mode) {
    FILE *f = ::fopen(filename, mode);
    return file_ptr(f);
}

/// mkdir
/// @return true if directory exists finally
inline bool mkdir(const std::filesystem::path& path) {
    std::error_code ec;
    return std::filesystem::create_directory(path, ec);
}

/// mkdir -p
/// @return true if directory exists finally
inline bool mkdirp(const std::filesystem::path& path) {
    std::error_code ec;
    return std::filesystem::create_directories(path, ec);
}

}  // namespace liph

#endif  // LIPH_FILE_H_
