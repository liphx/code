#ifndef LIPH_PRINT_H_
#define LIPH_PRINT_H_

#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
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

template <class T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& vc) {
    o << "[";
    std::string sep;
    for (const auto& x : vc) {
        o << sep << x;
        sep = ", ";
    }
    return o << "]";
}

template <class K, class V>
std::ostream& operator<<(std::ostream& o, const std::map<K, V>& map) {
    o << "{";
    std::string sep;
    for (const auto& x : map) {
        o << sep << x;
        sep = ", ";
    }
    return o << "}";
}

template <class K, class V>
std::ostream& operator<<(std::ostream& o, const std::unordered_map<K, V>& map) {
    o << "{";
    std::string sep;
    for (const auto& x : map) {
        o << sep << x;
        sep = ", ";
    }
    return o << "}";
}

inline void print() { std::cout << std::endl; }

inline void print(bool b) { std::cout << (b ? "true" : "false") << std::endl; }

template <class T>
void print(const T& t) {
    std::cout << t << std::endl;
}

template <class T, class... Args>
void print(T&& head, Args&&...args) {
    std::cout << head << ' ';
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
