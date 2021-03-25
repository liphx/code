#ifndef ALLOCATOR_HPP_
#define ALLOCATOR_HPP_

#include <cstddef>      // size_t, ptrdiff_t
#include <type_traits>  // true_type
#include <new>          // operator new, operator delete
#include <iostream>

namespace liph {

template<class T>
class allocator {
public:
    typedef T               value_type;
    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;
    typedef std::true_type  propagate_on_container_move_assignment;

    // 构造函数, 无状态分配器, 无任何操作
    constexpr allocator() noexcept = default;
    constexpr allocator(const allocator&) noexcept = default;
    template<class U>
    constexpr allocator(const allocator<U>&) noexcept {}
    allocator& operator=(const allocator&) noexcept = default;
    template<class U> // 如果不提供泛化的operator=, 会先调用泛化的copy constructor 隐式转换
    constexpr allocator& operator=(const allocator<U>&) noexcept { return *this; }

    ~allocator() noexcept {}

    constexpr T* allocate(std::size_t n) {
        return static_cast<T *>(::operator new (n * sizeof(T)));
    }
    constexpr void deallocate(T* p, std::size_t) {
        ::operator delete(p);
    }
};

template<class T1, class T2>
bool operator==(const allocator<T1>&, const allocator<T2>&) noexcept {
    return true; // 无状态, 始终返回 true
}

} // namespace liph;

#endif // ALLOCATOR_HPP_
