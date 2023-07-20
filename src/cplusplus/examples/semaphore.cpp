#if __GNUC__ >= 11
#include <semaphore>
std::binary_semaphore sem(0);
#endif

#include <thread>

#include "liph/liph.h"

using namespace std::literals;
std::mutex mtx;
std::condition_variable cv;

int main() {
#if __GNUC__ >= 11
    std::thread t([]() {
        liph::print(1);
        sem.release();  // sem +1
        std::this_thread::sleep_for(50ms);
        sem.acquire();  // wait and -1
        liph::print(4);
    });
    std::thread t2([]() {
        sem.acquire();  // sem init 0, wait and -1
        liph::print(2);
        std::this_thread::sleep_for(1s);
        liph::print(3);
        sem.release();  // +1
    });
    t.join();
    t2.join();
#endif

    std::cout << "=======" << std::endl;

    std::thread t3([]() {
        liph::print(1);
        std::this_thread::sleep_for(50ms);
        cv.notify_one();
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock);
        liph::print(4);
    });
    std::thread t4([]() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock);
        liph::print(2);
        std::this_thread::sleep_for(1s);
        liph::print(3);
        lock.unlock();  // if not unlock, will wait 5s
        cv.notify_one();
        std::this_thread::sleep_for(5s);
    });
    t3.join();
    t4.join();
}
