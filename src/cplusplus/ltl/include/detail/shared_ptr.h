#ifndef LTL_DETAIL_SHARED_PTR_H_
#define LTL_DETAIL_SHARED_PTR_H_

#include <memory>
#include <type_traits>

namespace ltl {

/*
 * shared_ptr 是通过指针保持对象共享所有权的智能指针
 * 多个 shared_ptr 对象可占有同一对象
 * 下列情况之一出现时销毁对象并解分配其内存
 *  - 最后剩下的占有对象的 shared_ptr 被销毁
 *  - 最后剩下的占有对象的 shared_ptr 被通过 operator= 或 reset() 赋值为另一指针
 */
template <class T>
class shared_ptr {
public:
    // typedef
    typedef std::remove_extent_t<T> element_type;
    typedef std::weak_ptr<T> weak_type;

    // constructor
    constexpr shared_ptr() noexcept {}                // 1
    constexpr shared_ptr(std::nullptr_t) noexcept {}  // 2
    template <class Y>
    explicit shared_ptr(Y *ptr);  // 3
    template <class Y, class Deleter>
    shared_ptr(Y *ptr, Deleter d);  // 4
    template <class Deleter>
    shared_ptr(std::nullptr_t ptr, Deleter d);  // 5
    template <class Y, class Deleter, class Alloc>
    shared_ptr(Y *ptr, Deleter d, Alloc alloc);  // 6
    template <class Deleter, class Alloc>
    shared_ptr(std::nullptr_t ptr, Deleter d, Alloc alloc);  // 7
    template <class Y>
    shared_ptr(const shared_ptr<Y>& r, element_type *ptr) noexcept;  // 8
    template <class Y>
    shared_ptr(shared_ptr<Y>&& r, element_type *ptr) noexcept;  // 8
    shared_ptr(const shared_ptr& r) noexcept;                   // 9
    template <class Y>
    shared_ptr(const shared_ptr<Y>& r) noexcept;  // 9
    shared_ptr(shared_ptr&& r) noexcept;          // 10
    template <class Y>
    shared_ptr(shared_ptr<Y>&& r) noexcept;  // 10
    template <class Y>
    explicit shared_ptr(const std::weak_ptr<Y>& r);  // 11
    template <class Y, class Deleter>
    shared_ptr(std::unique_ptr<Y, Deleter>&& r);  // 12

    // operator=
    shared_ptr& operator=(const shared_ptr& r) noexcept;
    template <class Y>
    shared_ptr& operator=(const shared_ptr<Y>& r) noexcept;
    shared_ptr& operator=(shared_ptr&& r) noexcept;
    template <class Y>
    shared_ptr& operator=(shared_ptr<Y>&& r) noexcept;
    template <class Y, class Deleter>
    shared_ptr& operator=(std::unique_ptr<Y, Deleter>&& r);

    // reset
    void reset() noexcept {}
    template <class Y>
    void reset(Y *ptr) {}
    template <class Y, class Deleter>
    void reset(Y *ptr, Deleter d);
    template <class Y, class Deleter, class Alloc>
    void reset(Y *ptr, Deleter d, Alloc alloc);

    // operator*, operator->
    T& operator*() const noexcept;
    T *operator->() const noexcept;

    ~shared_ptr() {}

    long use_count() const noexcept;

private:
};

// make_shared
template <class T, class... Args>
shared_ptr<T> make_shared(Args&&...args);
template <class T>
shared_ptr<T> make_shared(std::size_t N);
template <class T>
shared_ptr<T> make_shared();
template <class T>
shared_ptr<T> make_shared(std::size_t N, const std::remove_extent_t<T>& u);
template <class T>
shared_ptr<T> make_shared(const std::remove_extent_t<T>& u);
template <class T>
shared_ptr<T> make_shared_for_overwrite();
template <class T>
shared_ptr<T> make_shared_for_overwrite(std::size_t N);

}  // namespace ltl

#endif  // LTL_DETAIL_SHARED_PTR_H_
