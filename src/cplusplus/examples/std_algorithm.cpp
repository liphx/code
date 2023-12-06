#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <random>
#include <vector>

template <class Container>
void print(const Container& container) {
    std::cout << "[";
    bool first = true;
    for (const auto& x : container) {
        if (!first) std::cout << ",";
        std::cout << x;
        first = false;
    }
    std::cout << "]\n";
}

void example1() {
    std::vector vc{1, 2, 3, 4, 5};
    std::vector<int> vc2(5);
    std::copy(vc.begin(), vc.end(), vc2.begin());

    std::vector<int> vc3;
    // add elements to the end of the container
    std::copy(vc.begin(), vc.end(), std::back_inserter(vc3));
    print(vc3);  // [1,2,3,4,5]

    std::vector<int> vc4;
    auto less3 = [](int x) { return x < 3; };
    std::remove_copy_if(vc.begin(), vc.end(), std::back_inserter(vc4), less3);
    print(vc4);  // [3,4,5]

    std::vector<int> vc5;
    std::replace_copy_if(vc.begin(), vc.end(), std::back_inserter(vc5), less3, -1);
    print(vc5);  // [-1,-1,3,4,5]

    std::replace_if(vc.begin(), vc.end(), less3, -1);
    print(vc);  // [-1,-1,3,4,5]

    std::copy(vc.begin(), vc.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    // -1 -1 3 4 5
}

void example2() {
    std::vector<int> data(10);
    std::fill(data.begin(), data.end(), 7);
    print(data);  // [7,7,7,7,7,7,7,7,7,7]
    std::fill_n(data.begin(), 5, 3);
    print(data);  // [3,3,3,3,3,7,7,7,7,7]

    std::generate(data.begin(), data.end(), []() { return rand() % 10; });
    print(data);  // [7,9,3,8,0,2,4,8,3,9]
    std::generate_n(data.begin() + 2, 5, []() { return -1; });
    print(data);  // [7,9,-1,-1,-1,-1,-1,8,3,9]
}

void example3() {
    int data[8] = {0, 1, 2, 3, 3, 4, 4, 5};
    std::cout << std::count(data, data + 8, 3) << std::endl;                               // 2
    std::cout << std::count_if(data, data + 8, [](int x) { return x < 3; }) << std::endl;  // 3
}

void example4() {
    std::vector<int> data(5);
    int i = 1;
    std::generate_n(data.begin(), 4, [&i]() { return i++; });
    print(data);  // [1,2,3,4,0]
    std::vector<int> data_copy(data);
    std::copy(data_copy.begin(), data_copy.end() - 1, data_copy.begin() + 1);
    print(data_copy);  // [1,1,2,3,4]
                       // why not [1,1,1,1,1]
    std::copy_backward(data.begin(), data.end() - 1, data.end());
    print(data);  // [1,1,2,3,4]

    std::reverse(data.begin(), data.end());
    print(data);  // [4,3,2,1,1]

    std::vector<int> data_reverse(5);
    std::reverse_copy(data.begin(), data.end(), data_reverse.begin());
    print(data);          // [4,3,2,1,1]
    print(data_reverse);  // [1,1,2,3,4]

    std::vector<std::string> str1{"abc", "bcd"};
    std::vector<std::string> str2{"123", "234"};
    std::swap_ranges(str1.begin(), str1.end(), str2.begin());
    print(str1);  // [123,234]
    print(str2);  // [abc,bcd]

    // Performs a left rotation on a range of elements
    std::rotate(str1.begin(), str1.begin() + 1, str1.end());
    print(str1);  // [234,123]

    std::rotate_copy(str1.begin(), str1.begin() + 1, str1.end(), str2.begin());
    print(str1);  // [234,123]
    print(str2);  // [123,234]

    {
        std::string s("abc");
        std::vector ss{s};
        while (std::next_permutation(s.begin(), s.end())) {
            ss.emplace_back(s);
        }
        print(ss);  // [abc,acb,bac,bca,cab,cba]
    }
    {
        std::string s("cca");
        std::vector ss{s};
        while (std::prev_permutation(s.begin(), s.end())) {
            ss.emplace_back(s);
        }
        print(ss);  // [cca,cac,acc]
    }
    {
        std::string s("abcd");
        std::vector ss{s};
        for (int i = 0; i < 4; i++) {
            std::shuffle(s.begin(), s.end(), std::mt19937{std::random_device{}()});
            ss.emplace_back(s);
        }
        print(ss);  // [abcd,cbda,bcda,dcab,adbc]
    }

    std::vector<int> num{10, 5, 7, 13, 6, 24, 5};
    std::vector<int> num_copy(num);
    auto iter = std::partition(num.begin(), num.end(), [](int x) { return x % 2 != 0; });
    print(num);                       // [5,5,7,13,6,24,10]
    std::cout << *iter << std::endl;  // 6
    std::stable_partition(num_copy.begin(), num_copy.end(), [](int x) { return x % 2 != 0; });
    print(num_copy);  // [5,7,13,5,10,6,24]
}

void example5() {
    std::vector<int> data{1, 1, 2, -1, 5};
    auto iter = std::find(data.begin(), data.end(), -1);
    if (iter != data.end()) std::cout << *iter << std::endl;  // -1

    iter = std::find_if(data.begin(), data.end(), [](int x) { return x > 2; });
    if (iter != data.end()) std::cout << *iter << std::endl;  // 5

    iter = std::adjacent_find(data.begin(), data.end());
    if (iter != data.end()) std::cout << *iter << std::endl;  // 1

    iter = std::adjacent_find(data.begin(), data.end(), [](int x, int y) { return x + y > 3; });
    if (iter != data.end()) std::cout << *iter << std::endl;  // -1

    std::vector<std::string> str1{"eee", "bcd"};
    std::vector<std::string> str2{"bcd", "cde"};
    auto iter2 = find_first_of(str1.begin(), str1.end(), str2.begin(), str2.end());
    if (iter2 != str1.end()) std::cout << *iter2 << std::endl;  // bcd

    iter2 = std::find_first_of(
            str1.begin(), str1.end(), str2.begin(), str2.end(), [](std::string x, std::string y) { return x > y; });
    if (iter2 != str1.end()) std::cout << *iter2 << std::endl;  // eee

    iter2 = std::search(str1.begin(), str1.end(), str2.begin(), str2.begin());
    if (iter2 != str1.end()) std::cout << *iter2 << std::endl;  // eee

    iter2 = search(str1.begin(), str1.end(), str2.begin(), str2.begin() + 1);
    if (iter2 != str1.end()) std::cout << *iter2 << std::endl;  // bcd

    iter2 = search(str1.begin(), str1.end(), str2.begin(), str2.end());
    if (iter2 != str1.end()) std::cout << *iter2 << std::endl;

    iter2 = find_end(str1.begin(), str1.end(), str2.begin(), str2.begin() + 1);
    if (iter2 != str1.end()) std::cout << *iter2 << std::endl;  // bcd

    std::vector<bool> b{0, 1, 1, 0, 0, 0};
    auto iter3 = search_n(b.begin(), b.end(), 2, true);
    if (iter3 != b.end()) std::cout << *iter3 << std::endl;  // 1
    iter3 = search_n(b.begin(), b.end(), 3, false);
    if (iter3 != b.end()) std::cout << *iter3 << std::endl;  // 0

    std::string s("defabc");
    auto iter4 = min_element(s.begin(), s.end());
    std::cout << *iter4 << std::endl;  // a
    iter4 = max_element(s.begin(), s.end());
    std::cout << *iter4 << std::endl;  // f
}

void example6() {
    std::vector a{1, 2, 3, 4};
    std::vector b{0, 1, 2, 3};
    std::cout << equal(a.begin(), a.end(), b.begin()) << std::endl;          // 0
    std::cout << equal(a.begin(), a.end() - 1, b.begin() + 1) << std::endl;  // 1

    std::string s1("abcd");
    std::string s2("abce");
    std::cout << lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end()) << std::endl;      // 1
    std::cout << lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end() - 1) << std::endl;  // 0

    std::vector<int> c{1, 2, 3};
    std::vector<int> d{1, 3, 4};
    auto iter_pair = mismatch(c.begin(), c.end(), d.begin());
    if (iter_pair.first != c.end() && iter_pair.second != d.end())
        std::cout << *iter_pair.first << " " << *iter_pair.second << std::endl;
    // 2 3
}

void example7() {
    std::vector data{1, 1, -1, 0, 1, 2};
    auto iter = std::remove(data.begin(), data.end(), -1);
    print(data);
    data.erase(iter);
    print(data);
    data.erase(std::unique(data.begin(), data.end()), data.end());
    print(data);
    std::sort(data.begin(), data.end());
    data.erase(std::unique(data.begin(), data.end()), data.end());
    print(data);
}

void example8() {
    std::vector<int> data(10);
    std::generate(data.begin(), data.end(), []() { return rand() % 20; });
    print(data);
    std::vector<int> data_copy(data), data_copy2(data), data_copy3(data);
    std::sort(data.begin(), data.end());
    print(data);
    stable_sort(data_copy.begin(), data_copy.end());
    print(data_copy);
    std::partial_sort(data_copy2.begin(), data_copy2.begin() + 5, data_copy2.end());
    print(data_copy2);
    std::nth_element(data_copy3.begin(), data_copy3.begin() + 5, data_copy3.end());
    print(data_copy3);

    int a[3] = {1, 2, 3};
    int b[4] = {0, 1, 3, 4};
    std::vector<int> array;
    std::merge(a, a + 3, b, b + 4, std::back_inserter(array));
    print(array);
    std::rotate(array.begin(), array.begin() + 4, array.end());
    print(array);
    std::inplace_merge(array.begin(), array.begin() + 3, array.end());
    print(array);

    std::vector array2{0, 1, 1};
    std::cout << std::includes(array.begin(), array.end(), array2.begin(), array2.end()) << std::endl;  // 1

    std::vector array3{-2, -1, 0, 1, 1};
    std::set_union(
            array.begin(), array.end(), array3.begin(), array3.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    std::set_intersection(
            array.begin(), array.end(), array3.begin(), array3.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    std::set_difference(
            array.begin(), array.end(), array3.begin(), array3.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    std::set_symmetric_difference(
            array.begin(), array.end(), array3.begin(), array3.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

void example9() {
    std::vector<int> data{14, 5, 13, 26, 4, 18, 47, 26};

    std::make_heap(data.begin(), data.end(), std::greater<int>());
    print(data);
    std::make_heap(data.begin(), data.end());
    print(data);
    data.push_back(37);
    push_heap(data.begin(), data.end());
    print(data);
    std::pop_heap(begin(data), end(data));
    data.pop_back();
    print(data);
    std::cout << std::is_heap(begin(data), end(data)) << std::endl;  // 1
    if (std::is_heap(begin(data), end(data))) std::sort_heap(begin(data), end(data));
    print(data);
}

void example10() {
    std::vector<int> data{1, 2, 3, 4, 5};
    std::for_each(data.begin(), data.end(), [](int x) { std::cout << x << " "; });
    std::cout << std::endl;
    std::for_each(data.begin(), data.end(), [](int) { return 0; });
    print(data);

    std::vector<int> data2(5);
    std::transform(data.begin(), data.end(), data2.begin(), [](int x) { return x + 1; });
    print(data2);
    std::vector<int> data3(5);
    std::transform(data.begin(), data.end(), data2.begin(), data3.begin(), [](int x, int y) { return x * y; });
    print(data3);
}

void example11() {
    std::vector<int> data{1, 2, 3, 4, 5};
    std::cout << std::accumulate(data.begin(), data.end(), 0) << std::endl;
    std::cout << std::accumulate(data.begin(), data.end(), 0, [](int x, int y) { return x + y * y; }) << std::endl;
    std::vector<int> data2(data);
    std::cout << std::inner_product(data.begin(), data.end(), data2.begin(), 0) << std::endl;
    std::cout << std::inner_product(
                         data.begin(), data.end(), data2.begin(), 0, [](int x, int y) { return x + y; },
                         [](int x, int y) { return x * y; })
              << std::endl;

    std::vector<int> data3(5);
    std::partial_sum(data.begin(), data.end(), data3.begin());
    print(data3);
    std::partial_sum(data.begin(), data.end(), data3.begin(), std::multiplies<int>());
    print(data3);
    std::adjacent_difference(data.begin(), data.end(), data3.begin());
    print(data3);
    std::adjacent_difference(data.begin(), data.end(), data3.begin(), std::multiplies<int>());
    print(data3);
}

void example12() {
    auto a = std::make_pair(std::string("Hello"), 12);
    std::cout << a.first << " " << a.second << std::endl;
    std::vector<int> b(10);
    std::list<int> c(15);
    std::cout << distance(b.end(), b.begin()) << std::endl;
    std::cout << distance(c.begin(), c.end()) << std::endl;

    std::vector d{1, 2, 3};
    std::list<int> e;
    std::copy(d.begin(), d.end(), std::back_inserter(e));
    std::copy(e.begin(), e.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    std::copy(d.begin(), d.end(), front_inserter(e));
    std::copy(e.begin(), e.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    auto iter = e.begin();
    std::advance(iter, 2);
    std::copy(d.begin(), d.end(), std::inserter(e, iter));
    std::copy(e.begin(), e.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    std::string s1("abc"), s2("bc");
    std::cout << min(s1, s2, [](std::string x, std::string y) { return x.size() < y.size(); }) << std::endl;
    std::cout << max(s1, s2, [](std::string x, std::string y) { return x.at(0) < y.at(0); }) << std::endl;

    std::swap(s1, s2);
    std::cout << s1 << " " << s2 << std::endl;

    std::vector<int> data{0, 1};
    auto it1 = data.begin();
    auto it2 = data.begin() + 1;
    std::cout << distance(it1, it2) << std::endl;
    iter_swap(it1, it2);
    std::cout << *it1 << " " << *it2 << std::endl;
    std::cout << distance(it1, it2) << std::endl;
}

#define example(x)                                \
    std::cout << "=== example" << #x << " ===\n"; \
    example##x()

int main() {
    example(1);
    example(2);
    example(3);
    example(4);
    example(5);
    example(6);
    example(7);
    example(8);
    example(9);
    example(10);
    example(11);
    example(12);
}
