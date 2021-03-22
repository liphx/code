#include "vector.hpp"
#include <gtest/gtest.h>
using namespace std;

TEST(vector, constructor)
{
    liph::vector<int> data;
    EXPECT_EQ(data.size(), 0);
    ASSERT_TRUE(data.empty());
}

TEST(vector, push_back)
{
    liph::vector<int> data;
    for (int i = 0; i < 5; i++)
        data.push_back(i);
    for (int i = 0; i < 5; i++)
        EXPECT_EQ(data[i], i);
}


