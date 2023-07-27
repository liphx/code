#ifndef LIPH_CONTAINER_SKIP_LIST_H_
#define LIPH_CONTAINER_SKIP_LIST_H_

#include <functional>
#include <list>

#include "liph/lang/noncopyable.h"

namespace liph {

template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key> >
class skip_list : noncopyable {
public:
    class iterator;
    skip_list(const Compare& cmp = Compare(), const Allocator& alloc = Allocator()) : cmp_(cmp), alloc_(alloc) {}
    void insert(const Key& key);
    bool contains(const Key& key) const;
    bool empty() const { return size() == 0; }
    int32_t size() const { return head_.size; }
    int32_t max_size() const { return std::numeric_limits<int32_t>::max(); }
    void clear();
    iterator find(const Key& key);
    const iterator find(const Key& key) const;
    void erase(iterator pos);
    void erase(const Key& key);

public:
    iterator begin();
    const iterator begin() const;
    iterator end();
    const iterator end() const;

private:
    struct node_base {
        std::list<node_base *> next;
    };

    struct node_header : public node_base {
        int32_t size{0};
        node_header() { this->next = {this}; }
        node_base *base() { return this; }
    };

private:
    const Compare cmp_;
    const Allocator alloc_;
    node_header head_;
};

template <class K, class C, class A>
void skip_list<K, C, A>::insert(const K& key) {}

template <class K, class C, class A>
bool skip_list<K, C, A>::contains(const K& key) const {
    return false;
}

}  // namespace liph

#endif  // LIPH_CONTAINER_SKIP_LIST_H_
