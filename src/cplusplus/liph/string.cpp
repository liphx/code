#include "liph/string.h"

namespace liph {

std::string ltrim(const std::string& s, const std::string& charset) {
    auto start = s.find_first_not_of(charset);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string& s, const std::string& charset) {
    auto end = s.find_last_not_of(charset);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string& s, const std::string& charset) { return rtrim(ltrim(s, charset), charset); }

bool startswith(const std::string& s, const std::string& t) {
    return s.size() >= t.size() && std::equal(t.cbegin(), t.cend(), s.cbegin());
}

bool endswith(const std::string& s, const std::string& t) {
    return s.size() >= t.size() && std::equal(t.crbegin(), t.crend(), s.crbegin());
}

void split(std::vector<std::string>& tokens, const std::string& s, const std::string& delimiters) {
    auto last_pos = s.find_first_not_of(delimiters, 0);
    auto pos = s.find_first_of(delimiters, last_pos);
    while (pos != std::string::npos || last_pos != std::string::npos) {
        tokens.emplace_back(s.substr(last_pos, pos - last_pos));
        last_pos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, last_pos);
    }
}

std::vector<std::string> split(const std::string& s, const std::string& delimiters) {
    std::vector<std::string> tokens;
    split(tokens, s, delimiters);
    return tokens;
}

std::string basename(std::string_view path) {
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

std::string dirname(std::string_view path) {
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
