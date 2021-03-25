#include "vector.hpp"
#include <sstream>
#include <algorithm>
#include <vector>
#include <gtest/gtest.h>

TEST(vector, constructor)
{
    liph::vector<int> data;
    EXPECT_EQ(data.size(), 0);
    ASSERT_TRUE(data.empty());
}

TEST(vector, constructor2)
{
    int tmp[3] = {0, 1, 2};
    liph::vector<int> data(tmp, tmp + 3);
    EXPECT_EQ(data.size(), 3);
    for (int i = 0; i < 3; i++)
        EXPECT_EQ(data[i], i);

    std::vector<int> tmp2 {0, 1, 2};
    liph::vector<int> data2(tmp2.begin(), tmp2.end());
    EXPECT_EQ(data2.size(), 3);
    for (int i = 0; i < 3; i++)
        EXPECT_EQ(data2[i], i);
}

TEST(vector, constructor3)
{
    liph::vector<int> data {0, 1, 2};
    EXPECT_EQ(data.size(), 3);
    for (int i = 0; i < 3; i++)
        EXPECT_EQ(data[i], i);;
}


TEST(vector, push_back)
{
    liph::vector<int> data;
    for (int i = 0; i < 5; i++)
        data.push_back(i);
    for (int i = 0; i < 5; i++)
        EXPECT_EQ(data[i], i);
}

TEST(vector, at)
{
    liph::vector<int> data;
    EXPECT_THROW(data.at(0), std::out_of_range);
}

TEST(vector, data)
{
    liph::vector<int> data {0};
    int *p1 = data.data();
    *p1 = 1;
    EXPECT_EQ(data[0], 1);

    const liph::vector<int> data2(data);
    const int *p2 = data2.data();
    EXPECT_EQ(*p2, 1);
}

TEST(vector, ostream)
{
    std::ostringstream s;
    liph::vector<int> data;
    s << data;
    EXPECT_TRUE(s.str() == "[]");

    data.push_back(0);
    data.push_back(1);
    s.str("");
    s << data;
    EXPECT_TRUE(s.str() == "[0,1]");

    liph::vector<liph::vector<int>> data2;
    data2.push_back(data);
    s.str("");
    s << data2;
    EXPECT_TRUE(s.str() == "[[0,1]]");
}

TEST(vector, iterator)
{
    liph::vector<int> data {0,1,2,3,4};
    liph::vector<int> data2(5);
    std::copy(data.begin(), data.end(), data2.begin());
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(data2[i], i);
    }
}

TEST(vector, pop_back)
{
    struct A {
        A(){}
        ~A(){ std::cout << "~A()" << std::endl; }
    };

    liph::vector<A> data { A() };
    EXPECT_EQ(data.size(), 1);
    std::cout << "----" << std::endl;
    data.pop_back();
    EXPECT_EQ(data.size(), 0);
}
