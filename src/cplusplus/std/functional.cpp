#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector data{4, 17, 16, 3, 9, 36};
    // data[i] + 10
    std::transform(data.begin(), data.end(), data.begin(), std::bind(std::plus<int>(), std::placeholders::_1, 10));
    // [14, 27, 26, 13, 19, 46]
    // 20 > data[i]
    std::cout << std::count_if(data.begin(), data.end(), std::bind(std::greater<int>(), 20, std::placeholders::_1));
    // 3

    // !(data[i] == 14)
    std::cout << std::count_if(
            data.begin(), data.end(), std::not_fn(std::bind(std::equal_to<int>(), std::placeholders::_1, 14)));
    // 5

    std::transform(
            data.begin(), data.end(), std::ostream_iterator<bool>(std::cout, " "), [](int x) { return x % 2 == 0; });
    // 1 0 1 0 0 1
    std::cout << std::endl;

    std::transform(data.begin(), data.end(), data.begin(), [](int x) { return x * 2; });
    // [28, 54, 52, 26, 38, 92]

    std::vector<std::string> str{"abc", "123", "hello"};
    std::transform(str.begin(), str.end(), std::ostream_iterator<int>(std::cout, " "), std::mem_fn(&std::string::size));
    std::cout << std::endl;
    // 3 3 5

    std::vector<std::string *> str2{new std::string("abc"), new std::string("123"), new std::string("hello")};
    std::transform(
            str2.begin(), str2.end(), std::ostream_iterator<int>(std::cout, " "), std::mem_fn(&std::string::size));
    std::cout << std::endl;
    // 3 3 5
}
