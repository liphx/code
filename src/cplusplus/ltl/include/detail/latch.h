#ifndef LTL_DETAIL_LATCH_H_
#define LTL_DETAIL_LATCH_H_

#include <atomic>
#include <limits>

#include "cstddef"

namespace ltl {

class latch {
public:
    static constexpr ptrdiff_t max() noexcept { return std::numeric_limits<ptrdiff_t>::max(); }
    constexpr explicit latch(ptrdiff_t expected) : counter(expected) {}
    ~latch() = default;
    latch(const latch&) = delete;
    latch& operator=(const latch&) = delete;
    void count_down(ptrdiff_t update = 1) {
        const auto old = counter.fetch_sub(update, std::memory_order_release);
        if (old == update) {
            counter.notify_all();
        }
    }
    bool try_wait() const noexcept { return counter.load(std::memory_order_acquire) == 0; }
    void wait() const {
        while (!try_wait()) continue;
    }
    void arrive_and_wait(ptrdiff_t update = 1) {
        count_down(update);
        wait();
    }

private:
    std::atomic<ptrdiff_t> counter;
};

}  // namespace ltl

#endif  // LTL_DETAIL_LATCH_H_
