#ifndef LIPH_THREAD_H_
#define LIPH_THREAD_H_

#include <thread>

namespace liph {

class thread_guard {
public:
    explicit thread_guard(std::thread& t) : t_(t) {}
    ~thread_guard() {
        if (t_.joinable()) {
            t_.join();
        }
    }
    thread_guard(const thread_guard&) = delete;
    thread_guard& operator=(const thread_guard&) = delete;

private:
    std::thread& t_;
};

}  // namespace liph

#endif  // LIPH_THREAD_H_
