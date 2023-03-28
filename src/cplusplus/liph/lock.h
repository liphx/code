#ifndef LIPH_LOCK_H_
#define LIPH_LOCK_H_

#include <atomic>
#include <thread>

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
    std::atomic_flag v_ = ATOMIC_FLAG_INIT;

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

}  // namespace liph

#endif  // LIPH_LOCK_H_
