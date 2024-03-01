#ifndef LIPH_FORMAT_H_
#define LIPH_FORMAT_H_

// #if __has_cpp_attribute(__cpp_lib_format)
// #include <format>
// namespace liph {
// using std::format;
// }

// #else
#include "fmt/format.h"
#include "fmt/std.h"
namespace liph {
using fmt::format;
}

// #endif  // __cpp_lib_format

#endif  // LIPH_FORMAT_H_
