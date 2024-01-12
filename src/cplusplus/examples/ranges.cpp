#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector v{2, 1, 3, 9, 4, 5, 6, 8, 7};
    std::ranges::sort(v);
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';  // 1 2 3 4 5 6 7 8 9

    std::ranges::filter_view view{v, [](int x) { return x % 2 == 0; }};
    for (int i : view) std::cout << i << ' ';
    std::cout << '\n';  // 2 4 6 8

    std::ranges::take_view view2{view, 2};
    for (int i : view2) std::cout << i << ' ';
    std::cout << '\n';  // 2 4

    for (int i : std::ranges::take_view{std::ranges::filter_view{v, [](int x) { return x % 2 == 0; }}, 2})
        std::cout << i << ' ';
    std::cout << '\n';  // 2 4

    std::vector<std::pair<double, int>> vc{{1.0, 0}, {3.0, 0}, {4.0, 0}};
    double sum = 0;
    for (double x : std::views::elements<0>(vc)) sum += x;
    std::cout << "sum = " << sum << '\n';  // 8
}
