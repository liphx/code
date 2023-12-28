#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

#include "benchmark/benchmark.h"
#include "liph/string.h"

template <class Container>
std::string join2(const Container& data, std::string sep = " ") {
    std::string str, tmp;
    for (const auto& x : data) {
        str.append(tmp).append(std::to_string(x));
        if (!sep.empty()) tmp = std::move(sep);
    }
    return str;
}

template <class Container>
std::string join3(const Container& data, const std::string& sep = " ") {
    std::string str;
    for (auto it = data.begin(); it != data.end(); ++it)
        str.append(it == data.begin() ? "" : sep).append(std::to_string(*it));
    return str;
}

template <class Container>
std::string join4(const Container& data, const std::string& sep = " ") {
    return std::accumulate(data.begin(), data.end(), std::string{},
            [&sep](const std::string& str, const typename Container::value_type& x) {
                return str.empty() ? std::to_string(x) : str + sep + std::to_string(x);
            });
}

void print(const int *arr, int n) {
    for (int i = 0; i < n; i++) std::cout << arr[i] << ",\n"[i == n - 1];
}

// int main() {
//     const std::vector<int> data{1, 2, 4, 1, 9};
//     std::cout << liph::join(data, ",") << std::endl;
//     std::cout << join2(data, ",") << std::endl;
//     std::cout << join3(data, ",") << std::endl;
//     std::cout << join4(data, ",") << std::endl;
//     print(data.data(), data.size());
// }

const std::vector<int> v(1000);

static void bench1(benchmark::State& state) {
    for (auto _ : state) {
        liph::join(v, " ");
    }
}
static void bench2(benchmark::State& state) {
    for (auto _ : state) {
        join2(v, " ");
    }
}
static void bench3(benchmark::State& state) {
    for (auto _ : state) {
        join3(v, " ");
    }
}
static void bench4(benchmark::State& state) {
    for (auto _ : state) {
        join4(v, " ");
    }
}
BENCHMARK(bench1);
BENCHMARK(bench2);
BENCHMARK(bench3);
BENCHMARK(bench4);
BENCHMARK_MAIN();
