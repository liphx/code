#ifndef LIPH_CONCURRENCY_LOCK_H_
#define LIPH_CONCURRENCY_LOCK_H_

#include <atomic>
#include <thread>

// spinlock
// hierarchical_mutex

namespace liph {

class spinlock {
public:
    bool try_lock() noexcept { return !v_.test_and_set(std::memory_order_acquire); }

    void lock() noexcept {
        while (!try_lock()) {
            std::this_thread::yield();
        }
    }

    void unlock() noexcept { v_.clear(std::memory_order_release); }

private:
    std::atomic_flag v_ /* = ATOMIC_FLAG_INIT */;

public:
    class scoped_lock {
    private:
        spinlock& sp_;

        scoped_lock(scoped_lock const&);
        scoped_lock& operator=(scoped_lock const&);

    public:
        explicit scoped_lock(spinlock& sp) noexcept : sp_(sp) { sp.lock(); }

        ~scoped_lock() { sp_.unlock(); }
    };
};

class hierarchical_mutex {
public:
    explicit hierarchical_mutex(unsigned long value) : hierarchy_value_(value), previous_hierarchy_value_(0) {}
    void lock() {
        check_for_hierarchy_violation();
        mtx_.lock();
        update_hierarchy_value();
    }
    void unlock() {
        this_thread_hierarchy_value = previous_hierarchy_value_;
        mtx_.unlock();
    }
    bool try_lock() {
        check_for_hierarchy_violation();
        if (!mtx_.try_lock()) return false;
        update_hierarchy_value();
        return true;
    }

private:
    std::mutex mtx_;
    unsigned long const hierarchy_value_;
    unsigned long previous_hierarchy_value_;
    static thread_local unsigned long this_thread_hierarchy_value;

    void check_for_hierarchy_violation() {
        if (this_thread_hierarchy_value <= hierarchy_value_) {
            throw std::logic_error("mutex hierarchy violated");
        }
    }
    void update_hierarchy_value() {
        previous_hierarchy_value_ = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value_;
    }
};

inline thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(
        std::numeric_limits<unsigned long>::max());

}  // namespace liph

#endif  // LIPH_CONCURRENCY_LOCK_H_
