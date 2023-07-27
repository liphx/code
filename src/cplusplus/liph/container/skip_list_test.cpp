#include "liph/container/skip_list.h"
using namespace liph;

#include "gtest/gtest.h"

TEST(skip_list, skip_list) {
    skip_list<int> list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
    list.insert(0);
}
