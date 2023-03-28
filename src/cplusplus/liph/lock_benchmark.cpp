#include <mutex>
#include <thread>

#include "benchmark/benchmark.h"
#include "liph/lock.h"

constexpr int N = 100000;
constexpr int M = 8;

#define F()                                           \
    do {                                              \
        std::vector<std::thread> threads(M);          \
        for (int i = 0; i < M; i++) {                 \
            threads[i] = std::thread(f, std::ref(x)); \
        }                                             \
        for (int i = 0; i < M; i++) {                 \
            threads[i].join();                        \
        }                                             \
        assert(x == N * M);                           \
    } while (0);

static void bm_mutex(benchmark::State& state) {
    /* std::mutex mtx; */
    for (auto _ : state) {
        std::mutex mtx;
        auto f = [&mtx](int& x) {
            for (int i = 0; i < N; ++i) {
                std::lock_guard<std::mutex> lock(mtx);
                ++x;
            }
        };
        int x = 0;
        F();
    }
}

static void bm_spinlock(benchmark::State& state) {
    /* liph::spinlock sl; */
    for (auto _ : state) {
        liph::spinlock sl;
        auto f = [&sl](int& x) {
            for (int i = 0; i < N; ++i) {
                liph::spinlock::scoped_lock lock(sl);
                ++x;
            }
        };
        int x = 0;
        F();
    }
}

static void bm_atomic(benchmark::State& state) {
    for (auto _ : state) {
        auto f = [](std::atomic<int>& x) {
            for (int i = 0; i < N; ++i) {
                ++x;
            }
        };
        std::atomic<int> x = 0;
        F();
    }
}

BENCHMARK(bm_mutex);
BENCHMARK(bm_spinlock);
BENCHMARK(bm_atomic);

BENCHMARK_MAIN();
