#include "liph/format.h"

#include "gtest/gtest.h"

TEST(format, format) {
    EXPECT_EQ(liph::format("The answer is {}.", 42), "The answer is 42.");
    // ignore redundant parameters
    EXPECT_EQ(liph::format("{} {}", 1, 2, 3), "1 2");
}
