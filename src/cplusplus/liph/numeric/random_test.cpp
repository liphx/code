#include "liph/numeric/random.h"

#include "gtest/gtest.h"
#include "liph/logging.h"
#include "liph/string.h"

namespace liph {

TEST(random, random) {
    // std::cout << random::next_int32() << std::endl;
    // std::cout << random::next_uint32() << std::endl;
    // std::cout << random::next_int64() << std::endl;
    // std::cout << random::next_uint64() << std::endl;
    // std::cout << random::next_float() << std::endl;
    // std::cout << random::next_double() << std::endl;

    EXPECT_EQ(random::gen_int(-1), 0);
    EXPECT_EQ(random::gen_int(30, 29), 30);
    EXPECT_EQ(random::gen_int(40, 40), 40);
    EXPECT_EQ(random::gen_int(-31, -30), -31);
    EXPECT_EQ(random::gen_int(100, 101), 100);

    {
        int a = 30, b = 200;
        auto i = random::gen_int(a, b);
        EXPECT_TRUE(i >= a && i < b);
    }
    {
        int a = -30, b = -20;
        auto i = random::gen_int(a, b);
        EXPECT_TRUE(i >= a && i < b);
    }

    {
        double d = random::gen_real();
        EXPECT_TRUE(d >= 0 && d < 1);
    }
}

TEST(random, random_string) {
    auto str = liph::random_string(8);
    LOG << str;
    str = liph::ltrim(str, "0");
    if (str.empty()) {
        str = "0";
    }
    LOG << str;
}

}  // namespace liph
