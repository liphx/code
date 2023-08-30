#include "liph/liph.h"

int main() {
    std::vector<int> data{4, 17, 16, 3, 9, 36};
    // data[i] + 10
    std::transform(data.begin(), data.end(), data.begin(), std::bind(std::plus<int>(), std::placeholders::_1, 10));
    liph::print(data);
    // [14, 27, 26, 13, 19, 46]
    // 20 > data[i]
    liph::print(std::count_if(data.begin(), data.end(), std::bind(std::greater<int>(), 20, std::placeholders::_1)));
    // 3

    // !(data[i] == 14)
    liph::print(std::count_if(
            data.begin(), data.end(), std::not_fn(std::bind(std::equal_to<int>(), std::placeholders::_1, 14))));
    // 5

    std::transform(
            data.begin(), data.end(), std::ostream_iterator<bool>(std::cout, " "), [](int x) { return x % 2 == 0; });
    // 1 0 1 0 0 1
    std::cout << std::endl;

#if 0
    transform(
            data.begin(), data.end(), ostream_iterator<int>(cout, " "), bind(ptr_fun<double, int, double>(pow), std::placeholders::_1, 2));
    //=>16 289 256 9 81 1296
    cout << endl;

    vector<string> str{"abc", "123", "hello"};
    transform(str.begin(), str.end(), ostream_iterator<int>(cout, " "), mem_fun_ref(&string::size));
    //=>3 3 5
    cout << endl;

    vector<string *> str2{new string("abc"), new string("123"), new string("hello")};
    transform(str2.begin(), str2.end(), ostream_iterator<int>(cout, " "), mem_fun(&string::size));
    //=>3 3 5
    cout << endl;

#endif
}
