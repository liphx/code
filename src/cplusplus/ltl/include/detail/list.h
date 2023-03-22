#ifndef LTL_DETAIL_LIST_H_
#define LTL_DETAIL_LIST_H_

#include "iterator"
#include "memory"

namespace ltl {

template <class T, class Allocator = allocator<T>>
class list {
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
    class const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // construct/copy/destroy
    list() : list(Allocator()) {}
    explicit list(const Allocator& alloc) : alloc_(alloc) {}
    explicit list(size_type n, const Allocator& alloc = Allocator()) : list(n, T(), alloc) {}
    list(size_type n, const T& value, const Allocator& = Allocator()) {
        list_node_base *tmp = &head_;
        for (size_type i = 0; i < n; ++i) {
            list_node_base *node = create_list_node(value);
            node->prev = tmp;
            node->next = tmp->next;
            tmp->next->prev = node;
            tmp->next = node;
            tmp = node;
        }
        head_.size = n;
    }

    // avoid ambiguity
    template <class InputIterator,
            typename = std::enable_if_t<std::is_convertible_v<
                    typename std::iterator_traits<InputIterator>::iterator_category, input_iterator_tag>>>
    list(InputIterator first, InputIterator last, const Allocator& = Allocator()) {
        list_node_base *tmp = &head_;
        while (first != last) {
            list_node_base *node = create_list_node(*first);
            node->prev = tmp;
            node->next = tmp->next;
            tmp->next->prev = node;
            tmp->next = node;
            tmp = node;
            ++head_.size;
            ++first;
        }
    }
    list(const list& x) {
        list_node_base *tmp = &head_;
        for (auto it = x.begin(); it != x.end(); ++it) {
            list_node_base *node = create_list_node(*it);
            node->prev = tmp;
            node->next = tmp->next;
            tmp->next->prev = node;
            tmp->next = node;
            tmp = node;
        }
        head_.size = x.head_.size;
    }
    list(list&& x) {
        if (x.empty()) return;
        head_.size = x.head_.size;
        head_.next = x.head_.next;
        head_.prev = x.head_.prev;
        x.head_.size = 0;
        x.head_.next = x.head_base();
        x.head_.prev = x.head_base();
    }
    list(const list& x, const Allocator&) : list(x) {}
    list(list&& x, const Allocator&) : list(x) {}
    list(std::initializer_list<T> init, const Allocator& = Allocator()) {
        list_node_base *tmp = &head_;
        for (auto it = init.begin(); it != init.end(); ++it) {
            list_node_base *node = create_list_node(*it);
            node->prev = tmp;
            node->next = tmp->next;
            tmp->next->prev = node;
            tmp->next = node;
            tmp = node;
        }
        head_.size = init.size();
    }
    ~list() {}
    list& operator=(const list& x);
    list& operator=(list&& x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value);
    list& operator=(std::initializer_list<T>);
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last);
    void assign(size_type n, const T& t);
    void assign(std::initializer_list<T>);
    allocator_type get_allocator() const noexcept;

private:
    struct list_node_base {
        list_node_base *next;
        list_node_base *prev;
    };

    struct list_node_header : public list_node_base {
        size_type size;

        list_node_header() {
            size = 0;
            this->next = this->prev = this;
        }

        list_node_base *base() { return this; }
    };

    struct list_node : public list_node_base {
        value_type data;
        list_node_base *base() { return this; }
    };

    using T_alloc_type = typename std::allocator_traits<Allocator>::template rebind_alloc<T>;
    using T_alloc_traits = std::allocator_traits<T_alloc_type>;
    using node_alloc_type = typename T_alloc_traits::template rebind_alloc<list_node>;
    // using node_alloc_traits = std::allocator_traits<node_alloc_type>;

    list_node_base *create_list_node(const value_type& v) {
        auto *ptr = node_alloc_.allocate(1);
        construct_at(&ptr->data, v);
        return ptr->base();
    }

    template <class... Args>
    list_node_base *create_list_node(Args&&...args) {
        auto *ptr = node_alloc_.allocate(1);
        construct_at(&ptr->data, forward<Args>(args)...);
        return ptr->base();
    }

    void destroy_list_node(list_node_base *ptr) {
        list_node *node = static_cast<list_node *>(ptr);
        destroy_at(&node->data);
        node_alloc_.deallocate(node, 0);
    }

public:
    // iterators
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T *;
        using reference = T&;

        iterator(list_node_base *node = nullptr) : node(node) {}

        T& operator*() const { return static_cast<list_node *>(node)->data; }
        T *operator->() const { return &static_cast<list_node *>(node)->data; }
        iterator& operator++() {
            node = node->next;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }
        iterator& operator--() {
            node = node->prev;
            return *this;
        }
        iterator operator--(int) {
            iterator tmp = *this;
            --*this;
            return tmp;
        }
        bool operator!=(const iterator& rhs) const { return node != rhs.node; }
        bool operator==(const iterator& rhs) const { return node == rhs.node; }

        list_node_base *node;
    };

    class const_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T *;
        using reference = const T&;

        const_iterator(const list_node_base *node = nullptr) : node(node) {}
        const_iterator(const iterator& it) : node(it.node) {}

        reference operator*() const { return static_cast<const list_node *>(node)->data; }
        pointer operator->() const { return &static_cast<const list_node *>(node)->data; }
        const_iterator& operator++() {
            node = node->next;
            return *this;
        }
        const_iterator operator++(int) {
            iterator tmp = this;
            ++*this;
            return tmp;
        }
        const_iterator& operator--() {
            node = node->prev;
            return *this;
        }
        const_iterator operator--(int) {
            iterator tmp = this;
            --*this;
            return tmp;
        }
        iterator const_cast_() { return iterator(const_cast<list_node_base *>(node)); }

        bool operator!=(const const_iterator& rhs) const { return node != rhs.node; }
        bool operator==(const const_iterator& rhs) const { return node == rhs.node; }

        const list_node_base *node;
    };

    iterator begin() noexcept { return iterator(head_.next); }
    const_iterator begin() const noexcept { return const_iterator(head_.next); }
    iterator end() noexcept { return iterator(head_.base()); }
    const_iterator end() const noexcept { return const_iterator(&head_); }
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // capacity
    [[nodiscard]] bool empty() const noexcept { return head_.size == 0; }
    size_type size() const noexcept { return head_.size; }
    size_type max_size() const noexcept { return size_type(-1); }
    void resize(size_type sz);
    void resize(size_type sz, const T& c);

    // element access
    reference front() { return *begin(); }
    const_reference front() const;
    reference back() { return *--end(); }
    const_reference back() const;

    // modifiers
    template <class... Args>
    reference emplace_front(Args&&...args) {
        list_node_base *node = create_list_node(forward<Args>(args)...);
        node->prev = &head_;
        node->next = head_.next;
        head_.next->prev = node;
        head_.next = node;
        ++head_.size;
        return front();
    }
    template <class... Args>
    reference emplace_back(Args&&...args) {
        list_node_base *node = create_list_node(forward<Args>(args)...);
        node->next = &head_;
        node->prev = head_.prev;
        head_.prev->next = node;
        head_.prev = node;
        ++head_.size;
        return back();
    }
    void push_front(const T& x) { emplace_front(x); }
    void push_front(T&& x) { push_front(x); }
    void pop_front();
    void push_back(const T& x) { emplace_back(x); }
    void push_back(T&& x) { push_back(x); }
    void pop_back();
    template <class... Args>
    iterator emplace(const_iterator position, Args&&...args);
    iterator insert(const_iterator position, const T& x) {
        iterator it = position.const_cast_();
        list_node_base *node = create_list_node(x);
        node->next = it.node;
        node->prev = it.node->prev;
        it.node->prev->next = node;
        it.node->prev = node;
        ++head_.size;
        return iterator(node);
    }
    iterator insert(const_iterator position, T&& x) { return insert(position, x); }
    iterator insert(const_iterator position, size_type n, const T& x);
    template <class InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last);
    iterator insert(const_iterator position, std::initializer_list<T> il) {
        iterator it = position.const_cast_();
        for (auto i = std::rbegin(il); i != std::rend(il); ++i) {
            it = insert(it, *i);
        }
        return it;
    }
    iterator erase(const_iterator position) {
        iterator it = position.const_cast_();
        list_node_base *next = it.node->next;
        it.node->prev->next = next;
        next->prev = it.node->prev;
        destroy_list_node(it.node);
        --head_.size;
        return iterator(next);
    }
    iterator erase(const_iterator position, const_iterator last);
    void swap(list&) noexcept(std::allocator_traits<Allocator>::is_always_equal::value);
    void clear() noexcept {
        if (empty()) return;
        list_node_base *node = head_.next;
        for (size_type i = 0; i < head_.size; ++i) {
            list_node_base *tmp = node;
            node = node->next;
            destroy_list_node(tmp);
        }
        head_.size = 0;
        head_.next = head_.prev = &head_;
    }

    // list operations
    void splice(const_iterator position, list& x);
    void splice(const_iterator position, list&& x);
    void splice(const_iterator position, list& x, const_iterator i);
    void splice(const_iterator position, list&& x, const_iterator i);
    void splice(const_iterator position, list& x, const_iterator first, const_iterator last);
    void splice(const_iterator position, list&& x, const_iterator first, const_iterator last);
    size_type remove(const T& value) {
        size_type n = 0;
        auto it = begin();
        while (it != end()) {
            if (*it == value) {
                it = erase(it);
                ++n;
            } else {
                ++it;
            }
        }
        return n;
    }
    template <class Predicate>
    size_type remove_if(Predicate pred) {
        size_type n = 0;
        auto it = begin();
        while (it != end()) {
            if (pred(*it)) {
                it = erase(it);
                ++n;
            } else {
                ++it;
            }
        }
        return n;
    }
    size_type unique() {
        size_type n = 0;
        value_type prev = front();
        auto it = ++begin();
        while (it != end()) {
            if (*it == prev) {
                it = erase(it);
                ++n;
            } else {
                prev = *it;
                ++it;
            }
        }
        return n;
    }
    template <class BinaryPredicate>
    size_type unique(BinaryPredicate binary_pred);
    void merge(list& x) { merge(x, std::less<value_type>()); }
    void merge(list&& x);
    template <class Compare>
    void merge(list& x, Compare comp) {
        auto it1 = begin();
        auto it2 = x.begin();
        while (it1 != end() && it2 != x.end()) {
            if (comp(*it2, *it1)) {
                it1 = insert(it1, *it2);
                it2 = x.erase(it2);
            } else {
                ++it1;
            }
        }
        while (it2 != x.end()) {
            insert(end(), *it2);
            it2 = x.erase(it2);
        }
    }
    template <class Compare>
    void merge(list&& x, Compare comp);
    void sort() { sort(std::less<value_type>()); }
    template <class Compare>
    void sort(Compare comp) {
        quick_sort_(begin(), end(), comp);
    }
    void reverse() noexcept {
        list_node_base *node = &head_;
        for (size_type i = 0; i <= head_.size; ++i) {
            list_node_base *next = node->next;
            node->next = node->prev;
            node->prev = next;
            node = next;
        }
    }

private:
    template <class Compare>
    iterator partition_(iterator first, iterator last, Compare comp) {
        value_type pivotkey = *first;
        --last;
        while (first != last) {
            while (first != last && !comp(*last, pivotkey)) --last;
            if (first != last) *first++ = *last;

            while (first != last && comp(*first, pivotkey)) ++first;
            if (first != last) *last-- = *first;
        }
        *first = pivotkey;
        return ++first;
    }

    template <class Compare>
    void quick_sort_(iterator first, iterator last, Compare comp) {
        if (distance(first, last) > 1) {
            iterator it = partition_(first, last, comp);
            quick_sort_(first, it, comp);
            quick_sort_(it, last, comp);
        }
    }

private:
    Allocator alloc_;
    node_alloc_type node_alloc_;
    list_node_header head_;
};

/* template <class InputIterator, class Allocator = allocator<iter - value - type<InputIterator>>> */
/* list(InputIterator, InputIterator, Allocator = Allocator()) -> list<iter - value - type<InputIterator>, Allocator>;
 */

// swap
template <class T, class Allocator>
void swap(list<T, Allocator>& x, list<T, Allocator>& y) noexcept(noexcept(x.swap(y)));

}  // namespace ltl

#endif  // LTL_DETAIL_LIST_H_
