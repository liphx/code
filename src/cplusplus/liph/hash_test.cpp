#include "liph/hash.h"

#include "gtest/gtest.h"

TEST(hash, bkdr_hash) {
    EXPECT_EQ(liph::bkdr_hash(""), 0U);
    EXPECT_EQ(liph::bkdr_hash("hello"), 99162322U);
}
