#ifndef LIPH_FILE_H_
#define LIPH_FILE_H_

#include <cstdio>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace liph {

// read all from pathname, append to output
[[nodiscard]] bool read_file(const std::string& pathname, std::string& output);

// write all to pathname
[[nodiscard]] bool write_file(const std::string& pathname, const std::string& str);

// check path(file or directory) exists
[[nodiscard]] bool path_exists(const std::string& pathname);

// list all files(including directories) in pathname recursively and sort by names
[[nodiscard]] std::vector<std::string> list_files(const std::string& pathname);

using file_ptr = std::unique_ptr<FILE, decltype([](FILE *f) {
    if (f) fclose(f);
})>;

inline file_ptr fopen(char const *filename, char const *mode) {
    FILE *f = ::fopen(filename, mode);
    return file_ptr(f);
}

}  // namespace liph

#endif  // LIPH_FILE_H_
