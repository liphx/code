#include <iostream>

#include "boost/circular_buffer.hpp"

// circular_buffer: fixed capacity

int main() {
    {
        boost::circular_buffer<int> cb;
        // cb.empty() == true
        // cb.size() == 0
        cb.push_back(0);  // no insert
        // cb.empty() == true
    }

    {
        boost::circular_buffer<int> cb(3);
        std::cout << cb.empty() << std::endl;     // true
        std::cout << cb.size() << std::endl;      // 0
        std::cout << cb.capacity() << std::endl;  // 3
        std::cout << cb.front() << std::endl;     // random
        cb.push_back(0);
        cb.push_back(1);
        cb.push_back(2);
        // 0        1           2
        // begin                        end
        std::cout << cb.front() << "," << cb.back() << std::endl;  // 0,2
        cb.push_back(3);
        // 3        1           2
        //          begin,end
        std::cout << cb.front() << "," << cb.back() << std::endl;  // 1,3

        cb.resize(4);
        // 1        2           3       0
        // begin                                end
        for (auto it = cb.begin(); it != cb.end(); ++it) {
            std::cout << *it << std::endl;
        }
        std::cout << cb.size() << std::endl;                       // 4
        std::cout << cb.front() << "," << cb.back() << std::endl;  // 1,0
        cb.push_back(4);
        // 4        2           3       0
        //          begin,end
        std::cout << cb.front() << "," << cb.back() << std::endl;  // 2,4
    }
}
