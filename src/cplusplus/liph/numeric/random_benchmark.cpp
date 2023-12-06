#include "benchmark/benchmark.h"
#include "liph/numeric/random.h"

static void bench_std_rand(benchmark::State& state) {
    for (auto _ : state) {
        std::rand();
        std::rand();
    }
}

static void bench_random(benchmark::State& state) {
    for (auto _ : state) {
        liph::random::next_int64();
    }
}

BENCHMARK(bench_std_rand);
BENCHMARK(bench_random);

BENCHMARK_MAIN();
