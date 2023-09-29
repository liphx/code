#include "liph/lang/make_default.h"

#include <iostream>
#include <sstream>

#include "gtest/gtest.h"

namespace liph {

struct A {
    A(int) {}
    int n;
};

inline std::istream& operator>>(std::istream& is, A& a) { return is >> a.n; }

template <>
A make_default() {
    return A(0);
}

template <class T>
T get(std::istream& is) {
    // T value;
    T value = make_default<T>();
    is >> value;
    return value;
}

TEST(make_default, make_default) {
    std::string str("100 200");
    std::stringstream ss(str);
    EXPECT_EQ(get<int>(ss), 100);
    EXPECT_EQ(get<A>(ss).n, 200);
}

}  // namespace liph
