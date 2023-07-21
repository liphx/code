// boost/libs/circular_buffer/example/circular_buffer_bound_example.cpp
// https://www.boost.org/LICENSE_1_0.txt

#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>

#include "boost/circular_buffer.hpp"

template <class T>
class bounded_buffer {
public:
    using container_type = boost::circular_buffer<T>;
    using size_type = typename container_type::size_type;
    using value_type = typename container_type::value_type;
    using param_type = const value_type&;

    explicit bounded_buffer(size_type capacity) : unread_(0), container_(capacity) {}
    bounded_buffer(const bounded_buffer&) = delete;
    bounded_buffer& operator=(const bounded_buffer&) = delete;

    void push_front(param_type item) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, std::bind(&bounded_buffer<value_type>::is_not_full, this));
        container_.push_front(item);
        ++unread_;
        lock.unlock();
        not_empty_.notify_one();
    }

    void pop_back(value_type *ptr) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, std::bind(&bounded_buffer<value_type>::is_not_empty, this));
        *ptr = container_[--unread_];
        lock.unlock();
        not_full_.notify_one();
    }

private:
    bool is_not_empty() const { return unread_ > 0; }
    bool is_not_full() const { return unread_ < container_.capacity(); }

    size_type unread_;
    container_type container_;
    std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
};

#include <cstdio>
#include <thread>

const unsigned long queue_size = 100L;
const unsigned long total_elements = queue_size * 100L;

template <class Buffer>
class Producer {
    using value_type = typename Buffer::value_type;
    Buffer *container_;

public:
    Producer(Buffer *buffer) : container_(buffer) {}

    void operator()() {
        for (unsigned long i = 0L; i < total_elements; ++i) {
            printf("Producer %d\n", i);
            container_->push_front(value_type());
        }
    }
};

template <class Buffer>
class Consumer {
    using value_type = typename Buffer::value_type;
    Buffer *container_;
    value_type item_;

public:
    Consumer(Buffer *buffer) : container_(buffer) {}

    void operator()() {
        for (unsigned long i = 0L; i < total_elements; ++i) {
            printf("Consumer %d\n", i);
            container_->pop_back(&item_);
        }
    }
};

template <class Buffer>
void fifo_test(Buffer *buffer) {
    // Initialize the buffer with some values before launching producer and consumer threads.
    for (unsigned long i = queue_size / 2L; i > 0; --i) {
        buffer->push_front(typename Buffer::value_type());
    }

    Consumer<Buffer> consumer(buffer);
    Producer<Buffer> producer(buffer);
    std::thread consume(consumer);
    std::thread produce(producer);
    consume.join();
    produce.join();
}

int main() {
    bounded_buffer<int> bb_int(queue_size);
    fifo_test(&bb_int);
}
