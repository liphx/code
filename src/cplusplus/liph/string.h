#ifndef LIPH_STRING_H_
#define LIPH_STRING_H_

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace liph {

constexpr char WHITESPACE[] = " \n\r\t\f\v";

inline std::string ltrim(const std::string& s) {
    auto start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

inline std::string rtrim(const std::string& s) {
    auto end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

inline std::string trim(const std::string& s) { return rtrim(ltrim(s)); }

inline bool startswith(const std::string& s, const std::string& t) {
    return s.size() >= t.size() && std::equal(t.cbegin(), t.cend(), s.cbegin());
}

inline bool endswith(const std::string& s, const std::string& t) {
    return s.size() >= t.size() && std::equal(t.crbegin(), t.crend(), s.crbegin());
}

inline void split(std::vector<std::string>& tokens, const std::string& s, const std::string& delimiters = " ") {
    auto last_pos = s.find_first_not_of(delimiters, 0);
    auto pos = s.find_first_of(delimiters, last_pos);
    while (pos != std::string::npos || last_pos != std::string::npos) {
        tokens.emplace_back(s.substr(last_pos, pos - last_pos));
        last_pos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, last_pos);
    }
}

inline std::vector<std::string> split(const std::string& s, const std::string& delimiters = " ") {
    std::vector<std::string> tokens;
    split(tokens, s, delimiters);
    return tokens;
}

inline std::string basename(std::string_view path) {
    std::string ret;
    bool start = true;
    for (char ch : path) {
        if (ch != '/' && start) {
            ret += ch;
        } else if (ch == '/') {
            start = false;
        } else if (ch != '/' && !start) {
            ret.clear();
            ret += ch;
            start = true;
        }
    }
    if (!path.empty() && path.back() == '/' && ret.empty()) return "/";
    return ret;
}

inline std::string dirname(std::string_view path) {
    auto pos = path.find_last_not_of('/');
    if (pos == std::string_view::npos) return path.empty() ? "." : "/";  // empty or only contain '/'
    path = path.substr(0, pos + 1);                                      // remove the trailing '/'
    pos = path.find_last_of('/');
    if (pos == std::string_view::npos) return ".";  // not contain '/'
    path = path.substr(0, pos);                     // remove the trailing "/xxxx"
    if (path.empty()) return "/";                   // '/' or "////" before substr
    pos = path.find_last_not_of('/');
    if (pos != std::string_view::npos) path = path.substr(0, pos + 1);  // remove trailing duplicate '/'
    return std::string(path);
}

}  // namespace liph

#endif  // LIPH_STRING_H_
