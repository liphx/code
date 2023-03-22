#ifndef LTL_DETAIL_DEQUE_H_
#define LTL_DETAIL_DEQUE_H_

#include <iterator>
#include <memory>

#include "allocator.h"

namespace ltl {

template <class T, class Allocator = ltl::allocator<T>>
class deque {
public:
    // typedef
    typedef T value_type;
    typedef Allocator allocator_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef typename std::allocator_traits<Allocator>::pointer pointer;
    typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
    class iterator;
    class const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // constructor
    deque();
    explicit deque(const Allocator& alloc);
    deque(size_type count, const T& value, const Allocator& alloc = Allocator());
    explicit deque(size_type count, const Allocator& alloc = Allocator());
    template <class InputIt>
    deque(InputIt first, InputIt last, const Allocator& alloc = Allocator());
    deque(const deque& other);
    deque(const deque& other, const Allocator& alloc);
    deque(deque&& other);
    deque(deque&& other, const Allocator& alloc);
    deque(std::initializer_list<T> init, const Allocator& alloc = Allocator());

    ~deque();

    deque& operator=(const deque& other);
    deque& operator=(deque&& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value);
    deque& operator=(std::initializer_list<T> ilist);

    void assign(size_type count, const T& value);
    template <class InputIt>
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list<T> ilist);
    allocator_type get_allocator() const noexcept;

    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    [[nodiscard]] bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    void shrink_to_fit();

    void clear() noexcept;
    void push_front(const T& value);
    void push_front(T&& value);
    void push_back(const T& value);
    void push_back(T&& value);
    template <class... Args>
    reference emplace_back(Args&&...args);
    void pop_back();
    void pop_front();

private:
};

}  // namespace ltl

#endif  // LTL_DETAIL_DEQUE_H_
