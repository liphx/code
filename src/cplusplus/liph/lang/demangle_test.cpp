#include "liph/lang/demangle.h"

#include "gtest/gtest.h"
#include "liph/print.h"

namespace liph::demangle_test {

namespace a {
struct b {
    class c {};
};
}  // namespace a

void f() {}

TEST(demangle, demangle) {
    print(demangle<int>());
    print(demangle<const int>());
    print(demangle<std::string>());
    print(demangle<a::b::c>());
    const double d = 0;
    print(demangle(f));
}

}  // namespace liph::demangle_test
