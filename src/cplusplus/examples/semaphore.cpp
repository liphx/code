#include <condition_variable>
#include <mutex>
#include <semaphore>
#include <thread>

using namespace std::literals;

std::binary_semaphore sem(0);
std::mutex mtx;
std::condition_variable cv;

int main() {
    std::thread t([]() {
        puts("1");
        sem.release();  // sem +1
        std::this_thread::sleep_for(50ms);
        sem.acquire();  // wait and -1
        puts("4");
    });
    std::thread t2([]() {
        sem.acquire();  // sem init 0, wait and -1
        puts("2");
        std::this_thread::sleep_for(1s);
        puts("3");
        sem.release();  // +1
    });
    t.join();
    t2.join();

    puts("=======");

    std::thread t3([]() {
        puts("1");
        std::this_thread::sleep_for(50ms);
        cv.notify_one();
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock);
        puts("4");
    });
    std::thread t4([]() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock);
        puts("2");
        std::this_thread::sleep_for(1s);
        puts("3");
        lock.unlock();  // if not unlock, will wait 5s
        cv.notify_one();
        std::this_thread::sleep_for(5s);
    });
    t3.join();
    t4.join();
}
