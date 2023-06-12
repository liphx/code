#include <algorithm>
#include <cstdlib>
#include <vector>

#include "benchmark/benchmark.h"
#include "liph/sort.h"

std::vector<int> data(10000);
auto _ = []() {
    srand(time(0));
    std::generate(data.begin(), data.end(), []() { return rand() % 1000; });
    return 0;
}();

static void std_sort(benchmark::State& state) {
    // Perform setup here
    std::vector<int> tmp(data);
    for (auto _ : state) {
        // This code gets timed
        std::copy(data.begin(), data.end(), tmp.begin());
        std::sort(tmp.begin(), tmp.end());
        assert(std::is_sorted(tmp.begin(), tmp.end()));
    }
}

static void std_stable_sort(benchmark::State& state) {
    std::vector<int> tmp(data);
    for (auto _ : state) {
        std::copy(data.begin(), data.end(), tmp.begin());
        std::stable_sort(tmp.begin(), tmp.end());
        assert(std::is_sorted(tmp.begin(), tmp.end()));
    }
}

template <class Iter>
void std_merge_sort(Iter first, Iter last) {
    if (last - first > 1) {
        Iter middle = first + (last - first) / 2;
        std_merge_sort(first, middle);
        std_merge_sort(middle, last);
        std::inplace_merge(first, middle, last);
    }
}

static void std_merge_sort_by_inplace_merge(benchmark::State& state) {
    std::vector<int> tmp(data);
    for (auto _ : state) {
        std::copy(data.begin(), data.end(), tmp.begin());
        std_merge_sort(tmp.begin(), tmp.end());
        assert(std::is_sorted(tmp.begin(), tmp.end()));
    }
}

static void liph_quick_sort(benchmark::State& state) {
    std::vector<int> tmp(data);
    for (auto _ : state) {
        std::copy(data.begin(), data.end(), tmp.begin());
        liph::quick_sort(tmp.begin(), tmp.end());
        assert(std::is_sorted(tmp.begin(), tmp.end()));
    }
}

static void liph_bubble_sort(benchmark::State& state) {
    std::vector<int> tmp(data);
    for (auto _ : state) {
        std::copy(data.begin(), data.end(), tmp.begin());
        liph::bubble_sort(tmp.begin(), tmp.end());
        assert(std::is_sorted(tmp.begin(), tmp.end()));
    }
}

static void liph_selection_sort(benchmark::State& state) {
    std::vector<int> tmp(data);
    for (auto _ : state) {
        std::copy(data.begin(), data.end(), tmp.begin());
        liph::selection_sort(tmp.begin(), tmp.end());
        assert(std::is_sorted(tmp.begin(), tmp.end()));
    }
}

static void liph_merge_sort(benchmark::State& state) {
    std::vector<int> tmp(data);
    for (auto _ : state) {
        std::copy(data.begin(), data.end(), tmp.begin());
        liph::merge_sort(tmp.begin(), tmp.end());
        assert(std::is_sorted(tmp.begin(), tmp.end()));
    }
}

static void liph_insert_sort(benchmark::State& state) {
    std::vector<int> tmp(data);
    for (auto _ : state) {
        std::copy(data.begin(), data.end(), tmp.begin());
        liph::insert_sort(tmp.begin(), tmp.end());
        assert(std::is_sorted(tmp.begin(), tmp.end()));
    }
}

static void liph_shell_sort(benchmark::State& state) {
    std::vector<int> tmp(data);
    for (auto _ : state) {
        std::copy(data.begin(), data.end(), tmp.begin());
        liph::shell_sort(tmp.begin(), tmp.end());
        assert(std::is_sorted(tmp.begin(), tmp.end()));
    }
}

static void liph_counting_sort(benchmark::State& state) {
    std::vector<int> tmp(data);
    for (auto _ : state) {
        std::copy(data.begin(), data.end(), tmp.begin());
        liph::counting_sort(tmp.data(), tmp.size(), 1000);
        assert(std::is_sorted(tmp.begin(), tmp.end()));
    }
}

static void liph_radix_sort(benchmark::State& state) {
    std::vector<int> tmp(data);
    for (auto _ : state) {
        std::copy(data.begin(), data.end(), tmp.begin());
        liph::radix_sort(tmp.data(), tmp.size(), 3);
        assert(std::is_sorted(tmp.begin(), tmp.end()));
    }
}

static void liph_bucket_sort(benchmark::State& state) {
    std::vector<int> tmp(data);
    for (auto _ : state) {
        std::copy(data.begin(), data.end(), tmp.begin());
        liph::bucket_sort(tmp.data(), tmp.size(), 0, 1000, 16);
        assert(std::is_sorted(tmp.begin(), tmp.end()));
    }
}

// Register the function as a benchmark
BENCHMARK(std_sort);
BENCHMARK(std_stable_sort);
BENCHMARK(std_merge_sort_by_inplace_merge);
BENCHMARK(liph_quick_sort);
BENCHMARK(liph_bubble_sort);
BENCHMARK(liph_selection_sort);
BENCHMARK(liph_merge_sort);
BENCHMARK(liph_insert_sort);
BENCHMARK(liph_shell_sort);
BENCHMARK(liph_counting_sort);
BENCHMARK(liph_radix_sort);
BENCHMARK(liph_bucket_sort);

// Run the benchmark
BENCHMARK_MAIN();
