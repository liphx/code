#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

template <class T>
inline void random_sort(T arr[], int n, std::function<int()> rand = std::rand) {
    for (int i = 0; i < n; i++) {
        int j = rand() % (n - i);
        std::swap(arr[j], arr[n - i - 1]);
    }
}

template <class RandomIt>
inline void random_sort(RandomIt first, RandomIt last, std::function<int()> rand = std::rand) {
    return random_sort(first, std::distance(first, last), rand);
}

int main() {
    srand(time(nullptr));

#define print(arr)                           \
    for (int i : arr) std::cout << i << " "; \
    std::cout << std::endl

    int arr[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(int);
    random_sort(arr, n);
    print(arr);

    random_sort(arr, arr + n);
    print(arr);

    std::random_device rd;
    std::default_random_engine re(rd());
    std::uniform_int_distribution<int> distrib;
    auto rand = std::bind(distrib, re);
    random_sort(arr, n, rand);
    print(arr);

    random_sort(arr, arr + n, rand);
    print(arr);
}
