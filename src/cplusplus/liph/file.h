#ifndef LIPH_FILE_H_
#define LIPH_FILE_H_

#include <string>
#include <vector>

namespace liph {

// read all from pathname, append to output
bool read_file(const std::string& pathname, std::string& output);

// write all to pathname
bool write_file(const std::string& pathname, const std::string& str);

// check path(file or directory) exists
bool path_exists(const std::string& pathname);

// list all files(including directories) in pathname recursively and sort by names
std::vector<std::string> list_files(const std::string& pathname);

}  // namespace liph

#endif  // LIPH_FILE_H_
