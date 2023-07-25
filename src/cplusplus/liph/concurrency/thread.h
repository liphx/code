#ifndef LIPH_CONCURRENCY_THREAD_H_
#define LIPH_CONCURRENCY_THREAD_H_

#include <thread>

// thread_guard
// scoped_thread

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

class scoped_thread {
public:
    explicit scoped_thread(std::thread t_) : t(std::move(t_)) {
        if (!t.joinable()) throw std::logic_error("No thread");
    }
    ~scoped_thread() { t.join(); }
    scoped_thread(scoped_thread const&) = delete;
    scoped_thread& operator=(scoped_thread const&) = delete;

private:
    std::thread t;
};

}  // namespace liph

#endif  // LIPH_CONCURRENCY_THREAD_H_
