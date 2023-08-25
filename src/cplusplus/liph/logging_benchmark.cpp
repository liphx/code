#include "benchmark/benchmark.h"
#include "liph/logging.h"

std::vector<std::string> data;
auto _ = []() {
    data.resize(16000);
    std::generate(data.begin(), data.end(), []() { return std::string(128, '0'); });
    return 0;
}();
int thread_num = 16;

static void fstream(benchmark::State& state) {
    std::ofstream file("fstream.log");
    for (auto _ : state) {
        for (const auto& str : data) {
            file << '[' << liph::time_format() << ' ' << __FILENAME__ << ':' << __LINE__ << "] " << str << '\n';
        }
        file.flush();
    }
}

static void logging(benchmark::State& state) {
    for (auto _ : state) {
        for (const auto& str : data) {
            LOG << str;
        }
    }
}

static void fstream_mt(benchmark::State& state) {
    std::ofstream file("fstream_mt.log");
    std::mutex mtx;
    for (auto _ : state) {
        std::vector<std::thread> threads(thread_num);
        for (int i = 0; i < thread_num; i++) {
            threads[i] = std::thread(
                    [&](int idx) {
                        int n = data.size() / thread_num;
                        for (int j = n * idx; j < n + n * idx / thread_num; j++) {
                            std::lock_guard<std::mutex> lock(mtx);
                            file << '[' << liph::time_format() << ' ' << __FILENAME__ << ':' << __LINE__ << "] "
                                 << data[j] << '\n';
                        }
                    },
                    i);
        }
        for (int i = 0; i < thread_num; i++) {
            threads[i].join();
        }
    }
    file.close();
}

static void logging_mt(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<std::thread> threads(thread_num);
        for (int i = 0; i < thread_num; i++) {
            threads[i] = std::thread(
                    [](int idx) {
                        int n = data.size() / thread_num;
                        for (int j = n * idx; j < n + n * idx / thread_num; j++) {
                            LOG << data[j];
                        }
                    },
                    i);
        }
        for (int i = 0; i < thread_num; i++) {
            threads[i].join();
        }
    }
}

BENCHMARK(fstream);
BENCHMARK(logging);
BENCHMARK(fstream_mt);
BENCHMARK(logging_mt);

BENCHMARK_MAIN();
