#include "liph/double_buffer.h"

#include <numeric>
#include <thread>
#include <vector>

#include "gtest/gtest.h"
#include "liph/logging.h"

namespace liph {
namespace test {

DoubleBuffer<std::vector<int>> data;
DoubleBufferWithSharedPtr<std::vector<int>> data2;
constexpr int kLOOP = 1000;

TEST(double_buffer, DoubleBuffer) {
    std::thread t1([]() {
        int loop = kLOOP;
        while (loop-- > 0) {
            const auto& vc = data.Get();
            LOG << vc.size() << ", " << std::accumulate(vc.begin(), vc.end(), 0);
        }
    });

    std::thread t2([]() {
        int loop = kLOOP;
        while (loop-- > 0) {
            auto *vc = data.GetFree();
            vc->emplace_back(1);
            data.Switch();
        }
    });

    t1.join();
    t2.join();
    const auto& vc = data.Get();
    LOG << vc.size() << ", " << std::accumulate(vc.begin(), vc.end(), 0);
}

TEST(double_buffer, DoubleBufferWithSharedPtr) {
    std::thread t1([]() {
        int loop = kLOOP;
        while (loop-- > 0) {
            auto vc = data2.Get();
            if (vc == nullptr) continue;
            LOG << vc->size() << ", " << std::accumulate(vc->begin(), vc->end(), 0);
        }
    });

    std::thread t2([]() {
        int loop = kLOOP;
        while (loop-- > 0) {
            auto vc = data2.GetFree();
            vc->emplace_back(1);
            data2.Switch();
        }
    });

    t1.join();
    t2.join();
    auto vc = data2.Get();
    LOG << vc->size() << ", " << std::accumulate(vc->begin(), vc->end(), 0);
}

}  // namespace test
}  // namespace liph
