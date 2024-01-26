#include "liph/concurrency/threadpool.h"

namespace liph {

threadpool::threadpool(size_t size) : size_(size), stopped_(true), shutdown_(false) {
    if (size_ == 0) {
        size_ = std::thread::hardware_concurrency();
        if (size_ == 0) size_ = 1;
    }
}

threadpool::~threadpool() { stop(); }

void threadpool::start() {
    if (!threads_.empty()) return;  // already started
    stopped_ = false;
    for (size_t i = 0; i < size_; i++) threads_.emplace_back(std::thread(&threadpool::run, this));
}

void threadpool::stop() {
    if (stopped_) return;
    stopped_ = true;
    while (!tasks_.empty())
        ;
    {
        std::unique_lock<std::mutex> lock(lock_);  // need lock here
        shutdown_ = true;
        cv_.notify_all();
    }
    for (auto& t : threads_) {
        if (t.joinable()) t.join();
    }
}

void threadpool::run() {
    while (!shutdown_) {
        task t = take();
        if (t) {
            t();
        } else {
            std::unique_lock<std::mutex> lock(lock_);
            cv_.wait(lock, [this] -> bool { return shutdown_ || !tasks_.empty(); });
        }
    }
}

threadpool::task threadpool::take() {
    task ret;
    if (tasks_.pop(ret)) return ret;
    return nullptr;
}

}  // namespace liph
