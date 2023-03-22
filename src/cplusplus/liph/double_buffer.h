#ifndef LIPH_DOUBLE_BUFFER_H_
#define LIPH_DOUBLE_BUFFER_H_

#include <atomic>
#include <memory>

namespace liph {

template <class T>
class DoubleBuffer {
public:
    template <class... Args>
    DoubleBuffer(Args&&...args) : v0_(std::forward<Args>(args)...), v1_(std::forward<Args>(args)...) {}

    const T& Get() const { return *pv_[idx_]; }   // Reader
    T *GetFree() const { return pv_[1 - idx_]; }  // Writer
    void Switch() { idx_ = 1 - idx_; };           // Writer

private:
    T v0_;
    T v1_;
    T *pv_[2]{&v0_, &v1_};
    std::atomic<int> idx_{0};
};

#if __cplusplus < 202002L

template <class T>
class DoubleBufferWithSharedPtr {
public:
    std::shared_ptr<T> Get() const { return std::atomic_load(&v0_); }

    template <class... Args>
    std::shared_ptr<T> GetFree(Args&&...args) {
        v1_ = std::make_shared<T>(std::forward<Args>(args)...);
        return v1_;
    }

    void Switch() { std::atomic_store(&v0_, v1_); }

private:
    std::shared_ptr<T> v0_;
    std::shared_ptr<T> v1_;
};

#else  // C++20

template <class T>
class DoubleBufferWithSharedPtr {
public:
    template <class... Args>
    DoubleBufferWithSharedPtr(Args&&...args) : v0_(std::make_shared<T>(args...)) {}

    std::shared_ptr<T> Get() const { return v0_; }

    std::shared_ptr<T> GetFree() {
        v1_ = std::make_shared<T>(*(v0_.load()));
        return v1_;
    }

    void Switch() { v0_ = std::make_shared<T>(*(v1_.load())); }

private:
    std::atomic<std::shared_ptr<T> > v0_;
    std::atomic<std::shared_ptr<T> > v1_;
};

#endif

}  // namespace liph

#endif  // LIPH_DOUBLE_BUFFER_H_
