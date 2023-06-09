#ifndef PRIORITY_QUEUE_HPP_
#define PRIORITY_QUEUE_HPP_

#include <vector>
#include <functional>   // less
#include <algorithm>    // push_heap, pop_heap, make_heap

namespace liph {

/* 优先级队列 */
template<class T,
    class Container = std::vector<T>, 
    class Compare = std::less<typename Container::value_type>>
class priority_queue {
public:
    typedef Container                           container_type;
    typedef Compare                             value_compare;
    typedef typename Container::value_type      value_type;
    typedef typename Container::size_type       size_type;
    typedef typename Container::reference       reference;
    typedef typename Container::const_reference const_reference;

    /* constructor */
    priority_queue(): priority_queue(Compare(), Container()) {} // default constructor
    explicit priority_queue(const Compare& compare): priority_queue(compare, Container()) {}
    priority_queue(const Compare& compare, const Container& cont) {
        c = cont;
        comp = compare;
        std::make_heap(c.begin(), c.end(), comp);
    }
    template<class InputIt>
    priority_queue(InputIt first, InputIt last, const Compare& compare = Compare(), 
                                                 const Container& cont = Container()) {
        c = cont;
        comp = compare;
        c.insert(c.end(), first, last);
        std::make_heap(c.begin(), c.end(), comp);
    }

    bool empty() const { return c.size() == 0; }
    size_type size() const { return c.size(); }

    const_reference top() const { return c.front(); }

    void push(value_type&& value) {
        c.push_back(std::move(value));
        std::push_heap(c.begin(), c.end(), comp);
    }

    void pop() {
        std::pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
    }

private:
    Container c;
    Compare comp;
};

} // namespace liph

#endif // PRIORITY_QUEUE_HPP_
