#include "liph/sort.h"

#include <algorithm>
#include <forward_list>

#include "gtest/gtest.h"

TEST(sort, bubble_sort) {
    std::vector<int> data{49, 38, 65, 97, 76, 13, 27, 49, 0};
    liph::bubble_sort(data.begin(), data.end());
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end()));

    std::vector<int> data2{49};
    liph::bubble_sort(data2.begin(), data2.end());
    ASSERT_TRUE(std::is_sorted(data2.begin(), data2.end()));

    std::vector<int> data3;
    liph::bubble_sort(data3.begin(), data3.end());
    ASSERT_TRUE(std::is_sorted(data3.begin(), data3.end()));
}

TEST(sort, selection_sort) {
    std::vector<int> data{49, 38, 65, 97, 76, 13, 27, 49, 0};
    liph::selection_sort(data.begin(), data.end());
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end()));

    std::vector<int> data2{49};
    liph::selection_sort(data2.begin(), data2.end());
    ASSERT_TRUE(std::is_sorted(data2.begin(), data2.end()));

    std::vector<int> data3;
    liph::selection_sort(data3.begin(), data3.end());
    ASSERT_TRUE(std::is_sorted(data3.begin(), data3.end()));
}

TEST(sort, quick_sort) {
    std::vector<int> data{49, 38, 65, 97, 76, 13, 27, 49};
    liph::quick_sort(data.begin(), data.end());
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end()));

    std::vector<int> data2{49};
    liph::quick_sort(data2.begin(), data2.end());
    ASSERT_TRUE(std::is_sorted(data2.begin(), data2.end()));

    std::vector<int> data3;
    liph::quick_sort(data3.begin(), data3.end());
    ASSERT_TRUE(std::is_sorted(data3.begin(), data3.end()));
}

TEST(sort, merge_sort) {
    std::vector<int> data{49, 38, 65, 97, 76, 13, 27, 49};
    liph::merge_sort(data.begin(), data.end());
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end()));

    std::vector<int> data2{49};
    liph::merge_sort(data2.begin(), data2.end());
    ASSERT_TRUE(std::is_sorted(data2.begin(), data2.end()));

    std::vector<int> data3;
    liph::merge_sort(data3.begin(), data3.end());
    ASSERT_TRUE(std::is_sorted(data3.begin(), data3.end()));
}

TEST(sort, insert_sort) {
    std::vector<int> data{49, 38, 65, 97, 76, 13, 27, 49};
    liph::insert_sort(data.begin(), data.end());
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end()));

    std::vector<int> data2{49};
    liph::insert_sort(data2.begin(), data2.end());
    ASSERT_TRUE(std::is_sorted(data2.begin(), data2.end()));

    std::vector<int> data3;
    liph::insert_sort(data3.begin(), data3.end());
    ASSERT_TRUE(std::is_sorted(data3.begin(), data3.end()));
}

TEST(sort, shell_sort) {
    std::vector<int> data{49, 38, 65, 97, 76, 13, 27, 49};
    liph::shell_sort(data.begin(), data.end());
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end()));

    std::vector<int> data2{49};
    liph::shell_sort(data2.begin(), data2.end());
    ASSERT_TRUE(std::is_sorted(data2.begin(), data2.end()));

    std::vector<int> data3;
    liph::shell_sort(data3.begin(), data3.end());
    ASSERT_TRUE(std::is_sorted(data3.begin(), data3.end()));
}

TEST(sort, counting_sort) {
    int a[] = {9, 0, 8, 1, 9, 5, 7, 2};
    int n = sizeof(a) / sizeof(int);
    liph::counting_sort(a, n, 10);
    ASSERT_TRUE(std::is_sorted(a, a + n));
}

TEST(sort, radix_sort) {
    int a[] = {78, 0, 201, 78, 999, 84, 764, 201};
    int n = sizeof(a) / sizeof(int);
    liph::radix_sort(a, n, 3);
    ASSERT_TRUE(std::is_sorted(a, a + n));
}

TEST(sort, bucket_sort) {
    int a[] = {0, 78, 99, 26, 32, 14, 78, 62, 23, 58};
    int n = sizeof(a) / sizeof(int);
    liph::bucket_sort(a, n, 0, 100, 10);
    ASSERT_TRUE(std::is_sorted(a, a + n));

    int a2[] = {0};
    n = sizeof(a2) / sizeof(int);
    liph::bucket_sort(a2, n, 0, 1, 1);
    ASSERT_TRUE(std::is_sorted(a2, a2 + n));

    int a3[] = {0, 1, 2};
    n = sizeof(a3) / sizeof(int);
    liph::bucket_sort(a3, n, 0, 3, 2);
    ASSERT_TRUE(std::is_sorted(a3, a3 + n));
}
