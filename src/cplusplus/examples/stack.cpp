#include <iostream>
#include <list>
#include <stack>

// template <class T, class Container = std::deque<T> >
// class stack;

int main() {
    // use std::deque as the underlying container
    std::stack<int> data1;

    // use std::list as the underlying container
    std::stack<int, std::list<int>> data2;

    // construct by using another container
    std::list<int> data3{0, 1, 2};
    std::stack<int, std::list<int>> data4(data3);

    // copy constructor
    std::stack<int> data5{data1};

    // basic usage
    data5.push(0);
    data5.push(1);
    std::cout << data5.top() << std::endl;    // 1
    std::cout << data5.size() << std::endl;   // 2
    std::cout << data5.empty() << std::endl;  // 0
    data5.pop();
    std::cout << data5.top() << std::endl;  // 0
}
