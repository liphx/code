#include <iostream>

#include "boost/fiber/all.hpp"

boost::fibers::mutex mtx;
int count;

void loop() {
    for (int i = 0; i < 10000000; i++) {
        /* mtx.lock(); */
        count++;
        /* mtx.unlock(); */
    }
}

int main() {
    boost::fibers::fiber f(loop);
    boost::fibers::fiber f2(loop);
    f.join();
    f2.join();
    std::cout << "count = " << count << std::endl;
}
