#include "iterator"

#include "gtest/gtest.h"

TEST(iterator, advance) {
    int arr[3]{1, 2, 3};
    auto *it = &arr[0];
    ltl::advance(it, 2);
    EXPECT_EQ(*it, 3);
    ltl::advance(it, -1);
    EXPECT_EQ(*it, 2);
}

TEST(iterator, distance) {
    int arr[3]{};
    EXPECT_EQ(ltl::distance(&arr[0], &arr[2]), 2);
}
