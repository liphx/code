#ifndef LIPH_FILE_H_
#define LIPH_FILE_H_

#include <string>

namespace liph {

// read all from pathname, append to output
bool read_file(const std::string& pathname, std::string& output);

// write all to pathname
bool write_file(const std::string& pathname, const std::string& str);

}  // namespace liph

#endif  // LIPH_FILE_H_
