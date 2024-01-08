#ifndef LIPH_LANG_DEMANGLE_H_
#define LIPH_LANG_DEMANGLE_H_

#include <cxxabi.h>

#include <string>
#include <typeinfo>

namespace liph {

template <class T>
std::string demangle() {
    int status;
    const char *s = typeid(T).name();
    auto demangled = abi::__cxa_demangle(s, nullptr, nullptr, &status);
    std::string ret = demangled ?: s;
    free(demangled);
    return ret;
}

template <class T>
std::string demangle(const T& t) {
    int status;
    const char *s = typeid(t).name();
    auto demangled = abi::__cxa_demangle(s, nullptr, nullptr, &status);
    std::string ret = demangled ?: s;
    free(demangled);
    return ret;
}

}  // namespace liph

#endif  // LIPH_LANG_DEMANGLE_H_
