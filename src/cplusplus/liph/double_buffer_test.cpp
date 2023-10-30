#include "liph/double_buffer.h"

#include <numeric>
#include <thread>
#include <vector>

#include "gtest/gtest.h"
#include "liph/logging.h"

namespace liph {
namespace test {

double_buffer<std::vector<int>> data;
double_buffer_with_shared_ptr<std::vector<int>> data2;
constexpr int kLOOP = 1000;

TEST(double_buffer, double_buffer) {
    std::thread t1([]() {
        int loop = kLOOP;
        while (loop-- > 0) {
            const auto& vc = data.get();
            LOG << vc.size() << ", " << std::accumulate(vc.begin(), vc.end(), 0);
        }
    });

    std::thread t2([]() {
        int loop = kLOOP;
        while (loop-- > 0) {
            auto& vc = data.get_free();
            vc.emplace_back(1);
            data.swap();
        }
    });

    t1.join();
    t2.join();
    const auto& vc = data.get();
    LOG << vc.size() << ", " << std::accumulate(vc.begin(), vc.end(), 0);
}

TEST(double_buffer, double_buffer_with_shared_ptr) {
    std::thread t1([]() {
        int loop = kLOOP;
        while (loop-- > 0) {
            auto vc = data2.get();
            if (vc == nullptr) continue;
            LOG << vc->size() << ", " << std::accumulate(vc->begin(), vc->end(), 0);
        }
    });

    std::thread t2([]() {
        int loop = kLOOP;
        while (loop-- > 0) {
            auto vc = data2.get_free();
            vc->emplace_back(1);
            data2.swap();
        }
    });

    t1.join();
    t2.join();
    auto vc = data2.get();
    LOG << vc->size() << ", " << std::accumulate(vc->begin(), vc->end(), 0);
}

}  // namespace test
}  // namespace liph
