#include <iostream>
#include <vector>

void foo(int x) {
    if (auto y = x * x; y > 100) {
        std::cout << y << std::endl;
    } else {
        std::cout << x << std::endl;
    }
}

void bar(const std::vector<int>& v) {
    if (auto size = v.size()) {
        std::cout << "Not empty, size = " << size << std::endl;
    } else {
        std::cout << "Empty" << std::endl;
    }
}

int main() {
    foo(10);
    foo(20);
    bar({});
    bar({1, 2, 3});
}
