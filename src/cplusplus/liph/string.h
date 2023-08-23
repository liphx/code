#ifndef LIPH_STRING_H_
#define LIPH_STRING_H_

#include <string>
#include <string_view>
#include <vector>

namespace liph {

// remove whitespace(" \n\r\t\f\v") of s
std::string ltrim(const std::string& s);
std::string rtrim(const std::string& s);
std::string trim(const std::string& s);

bool startswith(const std::string& s, const std::string& t);
bool endswith(const std::string& s, const std::string& t);

void split(std::vector<std::string>& tokens, const std::string& s, const std::string& delimiters = " ");
std::vector<std::string> split(const std::string& s, const std::string& delimiters = " ");

std::string basename(std::string_view path);
std::string dirname(std::string_view path);

}  // namespace liph

#endif  // LIPH_STRING_H_
