#include <iostream>
#include <list>
#include <memory_resource>
#include <vector>

int main() {
    std::pmr::synchronized_pool_resource pool;
    std::pmr::list<int> l{&pool};
    for (int i = 0; i < 10; ++i) l.emplace_back(i);
    std::pmr::vector<int> v{l.size(), &pool};
    auto it = l.begin();
    for (int i = 0; i < 10; ++i) v[i] = *it++;
    for (int i = 0; i < 10; ++i) std::cout << i << '\n';
}
