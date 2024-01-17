#include <iostream>
#include <span>

void f(std::span<int> arr) {
    for (int& i : arr) i = 42;
}

void g(const std::span<int> arr) {
    int sum = 0;
    for (int i : arr) sum += i;
    std::cout << "size = " << arr.size() << " and sum = " << sum << '\n';
}

int main() {
    int arr[100];
    f(arr);
    g(arr);
    g({arr, 20});
}
