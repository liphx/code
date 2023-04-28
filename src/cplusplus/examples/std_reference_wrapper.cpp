// https://zh.cppreference.com/w/cpp/utility/functional/reference_wrapper

#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <vector>

int main() {
    std::list<int> l(10);
    std::iota(l.begin(), l.end(), -4);

    std::vector<std::reference_wrapper<int>> v(l.begin(), l.end());
    // 不能在 list 上用 shuffle （要求随机访问），但能在 vector 上使用它
    std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});

    std::cout << "Contents of the list: ";
    for (int n : l) std::cout << n << ' ';
    std::cout << '\n';

    std::cout << "Contents of the list, as seen through a shuffled vector: ";
    for (int i : v) std::cout << i << ' ';
    std::cout << '\n';

    std::cout << "Doubling the values in the initial list...\n";
    for (int& i : l) {
        i *= 2;
    }

    std::cout << "Contents of the list, as seen through a shuffled vector: ";
    for (int i : v) std::cout << i << ' ';
    std::cout << '\n';
}
