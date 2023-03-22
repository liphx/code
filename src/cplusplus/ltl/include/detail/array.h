#ifndef LTL_DETAIL_ARRAY_H_
#define LTL_DETAIL_ARRAY_H_

#include <iterator>
#include <stdexcept>
#include <string>

#include "cstddef"
#include "type_traits"

namespace ltl {

template <class T, size_t N>
struct array {
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using iterator = T *;
    using const_iterator = const iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    T elements[N];

    // aggregate type
    // array() = default;
    // ~array() = default;
    // array& operator=(const array<T, N>& other) = default;

    constexpr void fill(const T& value) {
        for (size_t i = 0; i < N; i++) elements[i] = value;
    }

    constexpr void swap(array&) noexcept(std::is_nothrow_swappable_v<T>);

    // capacity
    [[nodiscard]] constexpr bool empty() const noexcept { return N == 0; }
    constexpr size_type size() const noexcept { return N; }
    constexpr size_type max_size() const noexcept { return N; }

    // element access
    constexpr reference operator[](size_type n) { return elements[n]; }
    constexpr const_reference operator[](size_type n) const { return elements[n]; }
    constexpr reference at(size_type n) { return const_cast<reference>(static_cast<const array&>(*this).at(n)); }
    constexpr const_reference at(size_type n) const {
        if (n >= N) throw std::out_of_range("N is " + std::to_string(N));
        return elements[n];
    }
    constexpr reference front() { return elements[0]; }
    constexpr const_reference front() const { return elements[0]; }
    constexpr reference back() { return elements[N - 1]; }
    constexpr const_reference back() const { return elements[N - 1]; }

    // iterators
    constexpr iterator begin() noexcept { return iterator(data()); }
    constexpr const_iterator begin() const noexcept { return const_iterator(data()); }
    constexpr iterator end() noexcept { return iterator(data() + N); }
    constexpr const_iterator end() const noexcept { return const_iterator(data() + N); }
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    constexpr const_iterator cbegin() const noexcept { return begin(); }
    constexpr const_iterator cend() const noexcept { return end(); }
    constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    constexpr const_reverse_iterator crend() const noexcept { return rend(); }

    constexpr T *data() noexcept { return elements; }
    constexpr const T *data() const noexcept;

    friend constexpr bool operator==(const array&, const array&) = default;
};

template <size_t I, class T, size_t N>
constexpr T& get(ltl::array<T, N>& a) noexcept {
    return a[I];
}

};  // namespace ltl

#endif  // LTL_DETAIL_ARRAY_H_
