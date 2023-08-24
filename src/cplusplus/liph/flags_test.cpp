#include "liph/flags.h"

#include "gtest/gtest.h"

TEST(flags, flags) {
    std::vector<const char *> args{"a.out", "abc", "--msg", "hello", "--enable", "true", "--num", "90", "123"};
    liph::flags flags;
    flags.register_bool_flag("enable");
    flags.register_int32_flag("num");
    flags.register_string_flag("msg");

    char **argv = (char **)args.data();
    EXPECT_TRUE(flags.parse_flags(args.size(), &argv));
    EXPECT_TRUE(flags.bool_ref("enable"));
    EXPECT_EQ(flags.int32_ref("num"), 90);
    EXPECT_EQ(flags.string_ref("msg"), "hello");
}
