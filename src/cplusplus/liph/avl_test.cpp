#include "avl.h"

#include "gtest/gtest.h"

// g++ test_avl.cpp -lgtest -lgtest_main -lpthread -fsanitize=address -fsanitize-recover=address -lasan

TEST(avl, constructor) {
    liph::avl<int> avl;
    ASSERT_TRUE(avl.empty());
    EXPECT_EQ(avl.size(), 0);
}

TEST(avl, DISABLED_insert_erase_find) {
    liph::avl<int> avl;
    int x = 100;
    avl.insert(x);
    ASSERT_FALSE(avl.empty());
    EXPECT_EQ(avl.size(), 1);
    ASSERT_TRUE(avl.find(x));

    avl.erase(x);
    ASSERT_TRUE(avl.empty());
    EXPECT_EQ(avl.size(), 0);
    ASSERT_FALSE(avl.find(x));
}
