#ifndef LIPH_STRING_H_
#define LIPH_STRING_H_

#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace liph {

// default charset: whitespace(" \n\r\t\f\v")
std::string ltrim(const std::string& s, const std::string& charset = " \n\r\t\f\v");
std::string rtrim(const std::string& s, const std::string& charset = " \n\r\t\f\v");
std::string trim(const std::string& s, const std::string& charset = " \n\r\t\f\v");

bool startswith(const std::string& s, const std::string& t);
bool endswith(const std::string& s, const std::string& t);

void split(std::vector<std::string>& tokens, const std::string& s, const std::string& delimiters = " ");
std::vector<std::string> split(const std::string& s, const std::string& delimiters = " ");

std::string basename(std::string_view path);
std::string dirname(std::string_view path);

template <class T>
T from_string(const std::string& s) {
    std::istringstream is(s);
    T t;
    is >> t;
    return t;
}

template <class T>
std::string to_string(const T& t) {
    std::ostringstream os;
    os << t;
    return os.str();
}

}  // namespace liph

#endif  // LIPH_STRING_H_
