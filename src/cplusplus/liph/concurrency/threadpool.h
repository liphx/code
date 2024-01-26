#ifndef LIPH_CONCURRENCY_THREADPOOL_H_
#define LIPH_CONCURRENCY_THREADPOOL_H_

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

#include "liph/concurrency/blocking_queue.h"

namespace liph {

class threadpool {
public:
    threadpool(size_t size = 0);
    ~threadpool();

    threadpool(const threadpool&) = delete;
    threadpool(threadpool&&) = delete;
    threadpool& operator=(const threadpool&) = delete;
    threadpool& operator=(threadpool&&) = delete;

    void start();
    void stop();

    template <class Function, class... Args>
    void add(Function&& func, Args&&...args) {
        if (stopped_) throw std::runtime_error("Thread pool already stopped or not started");
        std::unique_lock<std::mutex> lock(lock_);  // need lock here
        tasks_.push(std::bind(func, std::forward<Args>(args)...));
        cv_.notify_one();
    }

    unsigned int size() const { return size_; }

private:
    using task = std::function<void()>;
    void run();
    task take();

private:
    unsigned int size_;
    std::atomic<bool> stopped_;
    std::atomic<bool> shutdown_;
    std::vector<std::thread> threads_;
    std::mutex lock_;
    std::condition_variable cv_;
    liph::blocking_queue<task> tasks_;
};

}  // namespace liph

#endif  // LIPH_CONCURRENCY_THREADPOOL_H_
