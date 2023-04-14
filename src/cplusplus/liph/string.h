#ifndef LIPH_STRING_H_
#define LIPH_STRING_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace liph {

constexpr char WHITESPACE[] = " \n\r\t\f\v";

static inline std::string ltrim(const std::string& s) {
    auto start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

static inline std::string rtrim(const std::string& s) {
    auto end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

static inline std::string trim(const std::string& s) { return rtrim(ltrim(s)); }

static inline bool startswith(const std::string& s, const std::string& t) {
    return s.size() >= t.size() && std::equal(t.cbegin(), t.cend(), s.cbegin());
}

static inline bool endswith(const std::string& s, const std::string& t) {
    return s.size() >= t.size() && std::equal(t.crbegin(), t.crend(), s.crbegin());
}

static inline void split(std::vector<std::string>& tokens, const std::string& s, const std::string& delimiters = " ") {
    auto last_pos = s.find_first_not_of(delimiters, 0);
    auto pos = s.find_first_of(delimiters, last_pos);
    while (pos != std::string::npos || last_pos != std::string::npos) {
        tokens.emplace_back(s.substr(last_pos, pos - last_pos));
        last_pos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, last_pos);
    }
}

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

static inline std::string base_name(const std::string& path) {
    auto pos = path.find_last_of('/');
    if (pos == std::string::npos) {
        return path;
    }
    return path.substr(pos + 1);
}

}  // namespace liph

#endif  // LIPH_STRING_H_