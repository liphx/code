#ifndef LTL_DETAIL_VECTOR_H_
#define LTL_DETAIL_VECTOR_H_

#include <algorithm>
#include <initializer_list>
#include <stdexcept>

#include "cstddef"
#include "iterator"
#include "memory"

namespace ltl {

template <class T, class Allocator = allocator<T>>
class vector {
public:
    // types
    using value_type = T;
    using allocator_type = Allocator;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    class iterator;
    using const_iterator = const iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // constructor
    constexpr vector() noexcept(noexcept(Allocator())) : vector(Allocator()) {}
    constexpr explicit vector(const Allocator& alloc) noexcept : alloc_(alloc), size_(0), capacity_(0) {}
    constexpr explicit vector(size_type capacity, const Allocator& alloc = Allocator())
            : alloc_(alloc), size_(0), capacity_(capacity) {
        if (capacity_ > 0) {
            data_ = alloc_.allocate(capacity_);
        }
    }
    constexpr vector(size_type capacity, const T& value, const Allocator& alloc = Allocator())
            : vector(capacity, alloc) {
        for (size_t i = 0; i < size_; ++i) {
            construct_at(data_ + i, value);
        }
    }
    template <class InputIterator>
    constexpr vector(InputIterator first, InputIterator last, const Allocator& = Allocator()) {
        capacity_ = size_ = ltl::distance(first, last);
        data_ = alloc_.allocate(capacity_);
        for (auto *p = data_; first != last; ++first, ++p) {
            construct_at(p, *first);
        }
    }
    constexpr vector(const vector<T>& other) {
        alloc_ = other.alloc_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        if (capacity_ > 0) {
            data_ = alloc_.allocate(capacity_);
        }
        for (size_t i = 0; i < size_; ++i) {
            construct_at(data_ + i, other[i]);
        }
    }
    constexpr vector(vector&& other) noexcept;
    constexpr vector(const vector&, const Allocator&);
    constexpr vector(vector&&, const Allocator&);
    constexpr vector(std::initializer_list<T> init, const Allocator& alloc = Allocator()) : alloc_(alloc) {
        capacity_ = size_ = init.size();
        data_ = alloc_.allocate(capacity_);
        auto it = init.begin();
        for (size_t i = 0; i < size_; ++i) {
            construct_at(data_ + i, *it++);
        }
    }

    // destructor
    ~vector() {
        for (size_t i = 0; i < size_; ++i) {
            destroy_at(data_ + i);
        }
        if (capacity_ > 0) {
            alloc_.deallocate(data_, 0);
        }
    }

    constexpr vector& operator=(const vector& other) {
        if (this == &other) return *this;
        if (capacity_ < other.capacity_) {
            reserve(other.capacity_);
        }
        size_ = other.size_;
        for (size_t i = 0; i < size_; ++i) {
            construct_at(data_ + i, other[i]);
        }
        return *this;
    }
    constexpr vector& operator=(vector&& x) noexcept(
            std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
            std::allocator_traits<Allocator>::is_always_equal::value);
    constexpr vector& operator=(std::initializer_list<T>);

    template <class InputIterator>
    constexpr void assign(InputIterator first, InputIterator last);
    constexpr void assign(size_type n, const T& u);
    constexpr void assign(std::initializer_list<T>);
    constexpr allocator_type get_allocator() const noexcept;

    // iterators
    class iterator {
    public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T *pointer;
        typedef T& reference;

        iterator(T *pos = nullptr) : pos_(pos) {}
        T& operator*() const { return *pos_; }
        T *operator->() const { return pos_; }
        iterator& operator++() {
            ++pos_;
            return *this;
        }
        iterator operator++(int) {
            iterator old = *this;
            ++pos_;
            return old;
        }
        iterator& operator--() {
            --pos_;
            return *this;
        }
        iterator operator--(int) {
            iterator old = *this;
            --pos_;
            return old;
        }
        bool operator!=(const iterator& rhs) const { return pos_ != rhs.pos_; }
        bool operator==(const iterator& rhs) const { return pos_ != rhs.pos_; }

    private:
        T *pos_;
    };

    constexpr iterator begin() noexcept { return iterator(data_); }
    constexpr const_iterator begin() const noexcept;
    constexpr iterator end() noexcept { return iterator(data_ + size_); }
    constexpr const_iterator end() const noexcept;
    constexpr reverse_iterator rbegin() noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr reverse_iterator rend() noexcept;
    constexpr const_reverse_iterator rend() const noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

    // capacity
    [[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }
    constexpr size_type size() const noexcept { return size_; }
    constexpr size_type max_size() const noexcept;
    constexpr size_type capacity() const noexcept { return capacity_; }
    constexpr void resize(size_type sz);
    constexpr void resize(size_type sz, const T& c);
    constexpr void reserve(size_type n) {
        if (n <= capacity_) return;
        if (capacity_ == 0) {
            capacity_ = n;
            data_ = alloc_.allocate(capacity_);
        } else {
            T *tmp = alloc_.allocate(n);
            if (tmp == nullptr) throw std::bad_alloc();
            for (size_t i = 0; i < size_; ++i) {
                construct_at(tmp + i, data_[i]);
            }
            alloc_.deallocate(data_, 0);
            for (size_t i = 0; i < size_; ++i) {
                destroy_at(data_ + i);
            }
            data_ = tmp;
            capacity_ = n;
        }
    }
    constexpr void shrink_to_fit();

    // element access
    constexpr reference operator[](size_type n) { return data_[n]; }
    constexpr const_reference operator[](size_type n) const { return data_[n]; }
    constexpr const_reference at(size_type n) const {
        if (n >= size_) throw std::out_of_range("argument >= this->size()");
        return data_[n];
    }
    constexpr reference at(size_type n) { return const_cast<reference>(static_cast<const vector&>(*this).at(n)); }
    constexpr reference front();
    constexpr const_reference front() const;
    constexpr reference back();
    constexpr const_reference back() const;

    // data access
    constexpr T *data() noexcept { return data_; }
    constexpr const T *data() const noexcept { return data_; }

    // modifiers
    template <class... Args>
    constexpr reference emplace_back(Args&&...args);
    constexpr void push_back(const T& x) {
        if (size_ < capacity_) {
            data_[size_++] = x;
        } else {
            if (capacity_ == 0)
                reserve(1);
            else
                reserve(capacity_ * 2);
            construct_at(data_ + size_, x);
            ++size_;
        }
    }
    constexpr void push_back(T&& x) {
        // todo
        T tmp = x;
        return push_back(tmp);
    }
    constexpr void pop_back() {
        if (size_ == 0) return;
        destroy_at(--size_ + data_);
    }

    template <class... Args>
    constexpr iterator emplace(const_iterator position, Args&&...args);
    constexpr iterator insert(const_iterator position, const T& x);
    constexpr iterator insert(const_iterator position, T&& x);
    constexpr iterator insert(const_iterator position, size_type n, const T& x);
    template <class InputIterator>
    constexpr iterator insert(const_iterator position, InputIterator first, InputIterator last);
    constexpr iterator insert(const_iterator position, std::initializer_list<T> il);
    constexpr iterator erase(const_iterator position);
    constexpr iterator erase(const_iterator first, const_iterator last);
    constexpr void swap(vector&) noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
                                          std::allocator_traits<Allocator>::is_always_equal::value);
    constexpr void clear() noexcept;

private:
    Allocator alloc_;
    T *data_;
    size_t size_;
    size_t capacity_;
};

template <class T, class Allocator>
constexpr bool operator==(const vector<T, Allocator>& x, const vector<T, Allocator>& y);

}  // namespace ltl

#endif  // LTL_DETAIL_VECTOR_H_
