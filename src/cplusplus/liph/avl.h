#ifndef LIPH_AVL_H_
#define LIPH_AVL_H_

#include <cstddef>     // size_t, ptrdiff_t
#include <functional>  // less

namespace liph {

template <class T, class Compare = std::less<T>>
class avl {
public:
    typedef T value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    avl() : tree(nullptr), node_num(0), comp(Compare()) {}

    ~avl() { clear(); }

    bool empty() const { return node_num == 0; }

    size_type size() const { return node_num; }

    bool find(const value_type& value) const {
        if (tree == nullptr) return false;
        return tree->find(value, comp);
    }

    void insert(const value_type& value) {
        if (tree == nullptr)
            tree = new node(value);
        else
            tree->insert(value, comp);

        node_num++;
    }

    void erase(const value_type& x) {
        if (tree) tree->erase(x, comp);
    }

    void clear() { delete tree; }

private:
    struct node {
        value_type value;
        node *left;
        node *right;
        int bf;

        node(const value_type& x) : value(x), left(nullptr), right(nullptr), bf(0) {}
        bool find(const value_type& x, const Compare& comp) const;
        void insert(const value_type& x, const Compare& comp);
        void erase(const value_type& x, const Compare& comp);
        void update();
        ~node() {
            delete left;
            delete right;
        }
    };

    node *tree;
    size_type node_num;
    Compare comp;
};

template <class T, class Compare>
bool avl<T, Compare>::node::find(const value_type& x, const Compare& comp) const {
    if (!comp(value, x) && !comp(x, value)) return true;
    if (comp(x, value) && left != nullptr) return left->find(x, comp);
    if (comp(value, x) && right != nullptr) return right->find(x, comp);
    return false;
}

template <class T, class Compare>
void avl<T, Compare>::node::insert(const value_type& x, const Compare& comp) {
    if (comp(x, value)) {
        if (left == nullptr) {
            left = new node(x);
            bf++;
        } else {
            left->insert(x, comp);
        }
    } else {
        if (right == nullptr) {
            right = new node(x);
            bf--;
        } else {
            right->insert(x, comp);
        }
    }

    update();
}

template <class T, class Compare>
void avl<T, Compare>::node::erase(const value_type& x, const Compare& comp) {}

template <class T, class Compare>
void avl<T, Compare>::node::update() {}

}  // namespace liph

#endif  // LIPH_AVL_H_
