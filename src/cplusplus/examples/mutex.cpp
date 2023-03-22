/*
- `std::mutex` 既不可复制亦不可移动
- `void lock()` 锁定互斥, 若其他线程已锁定则阻塞直到解锁
- `bool try_lock()` 尝试锁定, 若成功则锁定并返回true; 若失败直接返回false(标准允许mutex未被其他线程锁定但返回false)
- `void unlock()` 解锁
- `native_handle_type native_handle()` 返回os定义的原生句柄

UB

- 当前线程锁定`std::mutex`后调用`lock` 或 `try_lock`
- 当前线程未锁定`std::mutex`后调用`unlock`

## 使用 `std::lock_guard` 封装临界区的进入和退出

RAII风格

```cpp
void add(int x) {
        std::lock_guard<std::mutex> lock(mutex);
    vc.emplace_back(x);
}
```

- `lock_guard` 构造函数的参数为`mutex_type&`, 即mutex的引用, 在构造的时候 lock, 析构的时候 unlock
- `lock_guard` 不可复制
- `mutex`的生命周期必须长于`lock_guard`

## 作为类成员的mutex

```cpp
class Counter {
public:
    Counter() : value_(0) {}
    int64_t value() const;
    int64_t getAndIncrease();

private:
    int64_t value_;
    mutable std::mutex mutex_;
};

int64_t Counter::value() const {
    std::lock_guard<std::mutex> lock(mutex_);  // lock 的析构会晚于返回对象的构造，
    return value_;                             // 因此有效地保护了这个共享数据。
}

int64_t Counter::getAndIncrease() {
    std::lock_guard<std::mutex> lock(mutex_);
    int64_t ret = value_++;
    return ret;
}
```

`Counter::mutex_` 改为静态变量, 是否影响正确性和/或性能?
[Linux多线程服务端编程](https://book.douban.com/subject/20471211/)

- 不影响正确性
- 影响性能, 多个Counter实例每次只有一个对象可以读写value_

作为数据成员的 mutex 不能保护析构: 析构时, mutex本身正在销毁

## 防止死锁

```cpp
struct Box {
    explicit Box(int num) : num_things{num} {}

    int num_things;
    std::mutex m;
};

void transfer(Box& from, Box& to, int num) {
    // 仍未实际取锁
    std::unique_lock<std::mutex> lock1(from.m, std::defer_lock);
    std::unique_lock<std::mutex> lock2(to.m, std::defer_lock);

    // 锁两个 unique_lock 而不死锁
    std::lock(lock1, lock2);

    from.num_things -= num;
    to.num_things += num;

    // 'from.m' 与 'to.m' 互斥解锁于 'unique_lock' 析构函数
}
```

*/

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::vector<int> vc;
std::mutex mutex;

void add(int x) {
    mutex.lock();
    vc.emplace_back(x);
    mutex.unlock();
}

int main(int argc, char **argv) {
    std::thread t1(add, 1);
    std::thread t2(add, 2);
    t1.join();
    t2.join();

    std::cout << vc[0] << ", " << vc[1] << std::endl;
    // 1, 2 or 2, 1
}
