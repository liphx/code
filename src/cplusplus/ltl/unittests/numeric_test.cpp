#include "numeric"

#include <list>

#include "gtest/gtest.h"

TEST(numeric, accumulate) {
    int data[] = {1, 2, 3, 4, 5};
    EXPECT_EQ(ltl::accumulate(std::begin(data), std::end(data), 0), 15);
    EXPECT_EQ(ltl::accumulate(std::begin(data), std::end(data), 0, [](int init, int x) { return init + x * x; }), 55);

    std::string str = "hello world";
    EXPECT_EQ(ltl::accumulate(str.begin(), str.end(), 0, [](int32_t init, int32_t x) { return 31 * init + x; }),
            1794106052);
}

TEST(numeric, iota) {
    std::list<int> l(10);
    ltl::iota(l.begin(), l.end(), -4);
    EXPECT_EQ(l.back(), 5);
}

TEST(numeric, gcd_lcm) {
    constexpr int p{2 * 2 * 3};
    constexpr int q{2 * 3 * 3};
    static_assert(ltl::gcd(p, q) == 6);
    static_assert(ltl::lcm(p, q) == 36);

    int m = p, n = q;
    EXPECT_EQ(ltl::gcd(m, n), 6);
    EXPECT_EQ(ltl::lcm(m, n), 36);
}
