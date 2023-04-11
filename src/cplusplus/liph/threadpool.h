#ifndef LIPH_THREADPOOL_H_
#define LIPH_THREADPOOL_H_

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

#include "liph/blocking_queue.h"

namespace liph {

class ThreadPool {
public:
    ThreadPool(size_t size = 0) : size_(size), shutdown_(false) {
        if (size_ == 0) {
            size_ = std::thread::hardware_concurrency();
            if (size_ == 0) size_ = 1;
        }
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    ~ThreadPool() {
        Stop();
        cv_.notify_all();
        for (auto& t : threads_) {
            t.join();
        }
        threads_.clear();
    }

    void Start() {
        if (!threads_.empty()) return;  // already started
        for (size_t i = 0; i < size_; i++) {
            threads_.emplace_back(std::thread(&ThreadPool::Run, this));
        }
    }

    void Stop() { shutdown_ = true; }

    template <class Function, class... Args>
    void AddTask(Function&& func, Args&&...args) {
        if (shutdown_) {
            throw std::logic_error("Add task after thread pool shutdown");
        }
        auto entry = std::bind(func, std::forward<Args>(args)...);
        tasks_.push(entry);
        cv_.notify_one();
    }

    unsigned int Size() const { return size_; }

private:
    typedef std::function<void()> Task;

    void Run() {
        while (!shutdown_) {
            Task task = Take();
            if (task) {
                task();
            } else {
                std::unique_lock<std::mutex> lock(lock_);
                cv_.wait(lock);
            }
        }
    }

    Task Take() {
        if (tasks_.empty()) {
            return nullptr;
        }
        Task ret;
        tasks_.pop(ret);
        return ret;
    }

private:
    unsigned int size_;
    std::atomic<bool> shutdown_;
    std::vector<std::thread> threads_;
    std::mutex lock_;
    std::condition_variable cv_;
    liph::blocking_queue<Task> tasks_;
};

}  // namespace liph

#endif  // LIPH_THREADPOOL_H_
