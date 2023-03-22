#ifndef LTL_DETAIL_ALLOCATOR_H_
#define LTL_DETAIL_ALLOCATOR_H_

#include "cstddef"
#include "type_traits"

namespace ltl {

template <class T>
struct allocator {
public:
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef true_type propagate_on_container_move_assignment;

    // 构造函数, 无状态分配器, 无任何操作
    constexpr allocator() noexcept = default;
    constexpr allocator(const allocator&) noexcept = default;
    template <class U>
    constexpr allocator(const allocator<U>&) noexcept {}
    allocator& operator=(const allocator&) noexcept = default;

    // 如果不提供泛化的operator=, 会先调用泛化的copy constructor 隐式转换
    template <class U>
    constexpr allocator& operator=(const allocator<U>&) noexcept {
        return *this;
    }

    ~allocator() noexcept {}

    constexpr T *allocate(size_t n) { return static_cast<T *>(::operator new(n * sizeof(T))); }
    constexpr void deallocate(T *p, size_t) { ::operator delete(p); }
};

// 无状态, 始终返回 true
template <class T1, class T2>
bool operator==(const allocator<T1>&, const allocator<T2>&) noexcept {
    return true;
}

}  // namespace ltl

#endif  // LTL_DETAIL_ALLOCATOR_H_
