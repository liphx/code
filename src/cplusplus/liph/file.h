#ifndef LIPH_FILE_H_
#define LIPH_FILE_H_

#include <filesystem>
#include <string>

namespace liph {

// read all from pathname, append to output
bool read_file(const std::string& pathname, std::string& output);

// write all to pathname
bool write_file(const std::string& pathname, const std::string& str);

inline bool path_exists(const std::string& pathname) {
    std::error_code ec;
    return std::filesystem::exists(pathname, ec);
}

}  // namespace liph

#endif  // LIPH_FILE_H_
