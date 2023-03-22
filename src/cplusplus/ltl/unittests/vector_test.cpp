#include "vector"

#include <algorithm>
#include <sstream>
#include <vector>

#include "gtest/gtest.h"

// serialization
template <class T>
std::ostream& operator<<(std::ostream& o, const ltl::vector<T>& v) {
    o << '[';
    std::string sep;
    for (size_t i = 0; i < v.size(); i++) {
        o << sep << v[i];
        sep = ",";
    }
    o << ']';
    return o;
}

TEST(vector, constructor) {
    ltl::vector<int> data;
    EXPECT_EQ(data.size(), 0u);
    ASSERT_TRUE(data.empty());
}

TEST(vector, constructor2) {
    int tmp[3] = {0, 1, 2};
    ltl::vector<int> data(tmp, tmp + 3);
    EXPECT_EQ(data.size(), 3u);
    for (int i = 0; i < 3; i++) EXPECT_EQ(data[i], i);

    std::vector<int> tmp2{0, 1, 2};
    ltl::vector<int> data2(tmp2.begin(), tmp2.end());
    EXPECT_EQ(data2.size(), 3u);
    for (int i = 0; i < 3; i++) EXPECT_EQ(data2[i], i);
}

TEST(vector, constructor3) {
    ltl::vector<int> data{0, 1, 2};
    EXPECT_EQ(data.size(), 3u);
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(data[i], i);
    }
}

TEST(vector, push_back) {
    ltl::vector<int> data;
    for (int i = 0; i < 5; i++) data.push_back(i);
    for (int i = 0; i < 5; i++) EXPECT_EQ(data[i], i);
}

TEST(vector, at) {
    ltl::vector<int> data;
    EXPECT_THROW(data.at(0), std::out_of_range);
}

TEST(vector, data) {
    ltl::vector<int> data{0};
    int *p1 = data.data();
    *p1 = 1;
    EXPECT_EQ(data[0], 1);

    const ltl::vector<int> data2(data);
    const int *p2 = data2.data();
    EXPECT_EQ(*p2, 1);
}

TEST(vector, ostream) {
    std::ostringstream s;
    ltl::vector<int> data;
    s << data;
    EXPECT_TRUE(s.str() == "[]");

    data.push_back(0);
    data.push_back(1);
    s.str("");
    s << data;
    EXPECT_TRUE(s.str() == "[0,1]");

    ltl::vector<ltl::vector<int>> data2;
    data2.push_back(data);
    s.str("");
    s << data2;
    EXPECT_TRUE(s.str() == "[[0,1]]");
}

TEST(vector, iterator) {
    ltl::vector<int> data{0, 1, 2, 3, 4};
    ltl::vector<int> data2(5);
    std::copy(data.begin(), data.end(), data2.begin());
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(data2[i], i);
    }
}

TEST(vector, pop_back) {
    struct A {};
    ltl::vector<A> data{A()};
    EXPECT_EQ(data.size(), 1u);
    data.pop_back();
    EXPECT_EQ(data.size(), 0u);
}
