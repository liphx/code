#ifndef LIPH_LANG_NO_DESTRUCTOR_H_
#define LIPH_LANG_NO_DESTRUCTOR_H_

#include <utility>

namespace liph {

template <class T>
class no_destructor {
public:
    template <typename... Args>
    explicit no_destructor(Args&&...args) {
        new (storage_) T(std::forward<Args>(args)...);
    }

    explicit no_destructor(const T& x) { new (storage_) T(x); }
    explicit no_destructor(T&& x) { new (storage_) T(std::move(x)); }
    no_destructor(const no_destructor&) = delete;
    no_destructor& operator=(const no_destructor&) = delete;
    ~no_destructor() = default;

    const T *get() const { return reinterpret_cast<const T *>(storage_); }
    T *get() { return reinterpret_cast<T *>(storage_); }

    const T& operator*() const { return *get(); }
    T& operator*() { return *get(); }

    const T *operator->() const { return get(); }
    T *operator->() { return get(); }

private:
    char storage_[sizeof(T)];
};

}  // namespace liph

#endif  // LIPH_LANG_NO_DESTRUCTOR_H_
