#ifndef LIPH_PRINT_H_
#define LIPH_PRINT_H_

#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "liph/format.h"

namespace liph {

template <class T1, class T2>
std::ostream& operator<<(std::ostream& o, const std::pair<T1, T2>& p) {
    return o << "(" << p.first << ", " << p.second << ")";
}

inline std::ostream& operator<<(std::ostream& o, const std::vector<bool>& vb) {
    for (size_t i = 0; i < vb.size(); i++) o << vb[i];
    return o;
}

inline std::ostream& operator<<(std::ostream& o, const char *s) { return std::operator<<(o, s); }
inline std::ostream& operator<<(std::ostream& o, const std::string& s) { return std::operator<<(o, s); }
inline std::ostream& operator<<(std::ostream& o, std::string_view s) { return std::operator<<(o, s); }

template <std::ranges::input_range Range>
std::ostream& operator<<(std::ostream& o, const Range& range) {
    o << "[";
    bool first = true;
    for (const auto& value : range) {
        if (!first) o << ", ";
        first = false;
        o << value;
    }
    return o << "]";
}

template <class K, class V>
std::ostream& operator<<(std::ostream& o, const std::map<K, V>& map) {
    o << "{";
    bool first = true;
    for (const auto& value : map) {
        if (!first) o << ", ";
        first = false;
        o << value;
    }
    return o << "}";
}

template <class K, class V>
std::ostream& operator<<(std::ostream& o, const std::unordered_map<K, V>& map) {
    o << "{";
    bool first = true;
    for (const auto& value : map) {
        if (!first) o << ", ";
        first = false;
        o << value;
    }
    return o << "}";
}

inline void println() { std::cout << std::endl; }

inline void print(bool b) { std::cout << (b ? "true" : "false") << std::endl; }

template <class T>
concept Printable = requires(T t) { std::cout << t; };

template <Printable T>
void print(T&& t) {
    std::cout << t << std::endl;
}

template <Printable T, Printable... Args>
void print(T&& head, Args&&...args) {
    std::cout << head << ' ';
    print(args...);
}

template <Printable... Args>
void print(bool head, Args&&...args) {
    std::cout << (head ? "true" : "false") << ' ';
    print(args...);
}

}  // namespace liph

#ifndef __FILENAME__
#define __FILENAME__ ((strrchr(__FILE__, '/') ?: __FILE__ - 1) + 1)
#endif

#define SOURCE_LOCATION_FORMAT liph::format("[{}:{}]", __FILENAME__, __LINE__)
#define PRINT(...) liph::print(SOURCE_LOCATION_FORMAT, __VA_ARGS__)
#define DEBUG(x) liph::print(SOURCE_LOCATION_FORMAT, #x, "=", x)
#define TRACE(x)                             \
    liph::print(SOURCE_LOCATION_FORMAT, #x); \
    x

#endif  // LIPH_PRINT_H_
