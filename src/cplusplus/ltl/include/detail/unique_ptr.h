#ifndef LTL_DETAIL_UNIQUE_PTR_H_
#define LTL_DETAIL_UNIQUE_PTR_H_

#include "type_traits"
#include "utility"

namespace ltl {

template <class T>
struct default_delete {
    constexpr default_delete() noexcept = default;
    template <class U>
    default_delete(const default_delete<U>&) noexcept;
    void operator()(T *ptr) const { delete ptr; }
};

/*
 * unique_ptr 是通过指针占有并管理另一对象，并在 unique_ptr 离开作用域时释放该对象的智能指针
 * 在下列两者之一发生时用关联的删除器释放对象
 *  - 销毁了管理的 unique_ptr 对象
 *  - 通过 operator= 或 reset() 赋值另一指针给管理的 unique_ptr 对象
 */
template <class T, class D = default_delete<T>>
class unique_ptr {
public:
    using pointer = T *;
    using element_type = T;
    using deleter_type = D;

    // constructors
    constexpr unique_ptr() noexcept {}
    explicit unique_ptr(pointer p) noexcept : ptr_(p) {}
    unique_ptr(pointer p, const D& d) noexcept : ptr_(p), deleter_(d) {}
    unique_ptr(pointer p, remove_reference_t<D>&& d) noexcept : ptr_(p), deleter_(d) {}
    unique_ptr(unique_ptr&& other) noexcept : ptr_(other.ptr_), deleter_(other.deleter_) { other.ptr_ = nullptr; }
    constexpr unique_ptr(nullptr_t) noexcept {}
    template <class U, class E>
    unique_ptr(unique_ptr<U, E>&& u) noexcept;

    // destructor
    ~unique_ptr() { deleter_(ptr_); }

    // assignment
    // 从 r 转移所有权到 *this
    unique_ptr& operator=(unique_ptr&& r) noexcept {
        deleter_(ptr_);
        ptr_ = r.ptr_;
        r.ptr_ = nullptr;
        return *this;
    }
    template <class U, class E>
    unique_ptr& operator=(unique_ptr<U, E>&& r) noexcept;
    unique_ptr& operator=(std::nullptr_t) noexcept {
        deleter_(ptr_);
        ptr_ = nullptr;
        return *this;
    }

    // observers
    std::add_lvalue_reference_t<T> operator*() const { return *ptr_; }
    pointer operator->() const noexcept { return ptr_; }
    // 返回指向被管理对象的指针，如果无被管理对象，则为 nullptr
    pointer get() const noexcept { return ptr_; }
    deleter_type& get_deleter() noexcept { return deleter_; }
    const deleter_type& get_deleter() const noexcept { return deleter_; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    // modifiers
    // 若存在，则释放被管理对象的所有权，返回指向被管理对象的指针
    pointer release() noexcept {
        pointer ret = ptr_;
        ptr_ = nullptr;
        return ret;
    }
    // 替换被管理对象
    void reset(pointer p = pointer()) noexcept {
        deleter_(ptr_);
        ptr_ = p;
    }
    // 交换 *this 和 other 的被管理对象和关联的删除器
    void swap(unique_ptr& other) noexcept { ltl::swap(ptr_, other.ptr_); }

    // disable copy from lvalue
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

private:
    pointer ptr_{nullptr};
    D deleter_;
};

// 构造 T 类型对象并将其包装进 unique_ptr，可能抛出 std::bad_alloc 或任何 T 的构造函数所抛的异常
// 若抛出异常，则此函数无效果
template <typename T, typename... Args>
inline unique_ptr<T> make_unique(Args&&...args) {
    return unique_ptr<T>(new T(forward<Args>(args)...));
}

}  // namespace ltl

#endif  // LTL_DETAIL_UNIQUE_PTR_H_
