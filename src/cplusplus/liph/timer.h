#ifndef LIPH_TIMER_H_
#define LIPH_TIMER_H_

#include <queue>
#include <thread>

#include "liph/concurrency/lock.h"

namespace liph {

class timer {
public:
    timer() {}
    ~timer() { stop(); }

    void start() {
        running_ = true;
        thread_ = std::thread([&]() {
            int current_time = 0;
            while (running_) {
                std::unique_lock lock(lock_);
                if (queue_.empty()) {
                    unique_unlock unlock(lock);
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }

                job j = std::move(queue_.top());
                queue_.pop();
                unique_unlock unlock(lock);

                int sleep_time = j.next_time - current_time;
                if (sleep_time > 0) std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
                current_time = j.next_time;

                if (j.task) {
                    j.task();
                    j.next_time += j.interval_time;
                    add(std::move(j));
                }
            }
        });
    }

    void stop() {
        running_ = false;
        if (thread_.joinable()) thread_.join();
    }

    template <class Function, class... Args>
    void add(int interval, Function&& func, Args&&...args) {
        add(job{0, interval, std::bind(func, std::forward<Args>(args)...)});
    }

private:
    struct job {
        int next_time;
        int interval_time;
        std::function<void()> task;

        bool operator<(const job& other) const { return next_time > other.next_time; }
    };

    void add(job j) {
        std::unique_lock lock(lock_);
        queue_.push(std::move(j));
    }

    std::priority_queue<job> queue_;
    std::thread thread_;
    std::mutex lock_;
    std::atomic<bool> running_;
};

}  // namespace liph

#endif  // LIPH_TIMER_H_
