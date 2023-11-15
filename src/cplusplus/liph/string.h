#ifndef LIPH_STRING_H_
#define LIPH_STRING_H_

#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace liph {

/// Remove whitespace characters to the left of s
/// @param charset default: whitespace(" \n\r\t\f\v")
/// @return sub string_view of s
std::string_view ltrim(std::string_view s, std::string_view charset = " \n\r\t\f\v");

/// Remove whitespace characters to the right of s
/// @see ltrim
std::string_view rtrim(std::string_view s, std::string_view charset = " \n\r\t\f\v");

/// Remove whitespace characters to the left and right of s
/// @see ltrim
std::string_view trim(std::string_view s, std::string_view charset = " \n\r\t\f\v");

/// @return true if s starts with t
bool startswith(std::string_view s, std::string_view t);

/// @return true if s ends with t
bool endswith(std::string_view s, std::string_view t);

std::string replace(std::string_view str, std::string_view old, std::string_view n);

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

void skip_whitespace(std::string_view& sv);
bool eat_symbol(std::string_view& sv, std::string_view symbol);

std::optional<int> to_int(std::string_view);

}  // namespace liph

#endif  // LIPH_STRING_H_
