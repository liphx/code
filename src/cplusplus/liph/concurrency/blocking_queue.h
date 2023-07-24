#ifndef LIPH_CONCURRENCY_BLOCKING_QUEUE_H_
#define LIPH_CONCURRENCY_BLOCKING_QUEUE_H_

#include <mutex>
#include <queue>
#include <shared_mutex>

namespace liph {

template <class T>
class blocking_queue {
public:
    blocking_queue() = default;
    ~blocking_queue() = default;

    bool empty() const { return size() == 0; }

    size_t size() const {
        std::shared_lock<std::shared_mutex> rlock(mutex_);
        return queue_.size();
    }

    void push(const T& v) {
        std::unique_lock<std::shared_mutex> wlock(mutex_);
        queue_.push(v);
    }

    template <class... Args>
    void emplace(Args&&...args) {
        std::unique_lock<std::shared_mutex> wlock(mutex_);
        queue_.emplace(std::forward<Args>(args)...);
    }

    void pop() {
        std::unique_lock<std::shared_mutex> wlock(mutex_);
        queue_.pop();
    }

    bool pop(T& v) {
        std::unique_lock<std::shared_mutex> wlock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        v = std::move(queue_.front());
        queue_.pop();
        return true;
    }

private:
    std::queue<T> queue_;
    mutable std::shared_mutex mutex_;
};

}  // namespace liph

#endif  // LIPH_CONCURRENCY_BLOCKING_QUEUE_H_
