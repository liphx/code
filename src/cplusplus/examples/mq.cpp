#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
using namespace std;

const int k = 100;
const int m = 5;
const int n = 3;
queue<int> q;
mutex mtx;
mutex io;
condition_variable cv;
condition_variable cv2;

class producer {
public:
    void produce(int x) {
        unique_lock<mutex> lock(mtx);
        while (q.size() >= k) {
            cv.wait(lock);
        }
        q.emplace(x);
        if (q.size() == 1) {
            cv2.notify_one();
        }
    }
};

class consumer {
public:
    int consume() {
        int ret;
        unique_lock<mutex> lock(mtx);
        while (q.size() == 0) {
            cv2.wait(lock);
        }
        ret = q.front();
        q.pop();
        if (q.size() == k - 1) {
            cv.notify_one();
        }
        return ret;
    }
};

void produce(int i) {
    producer p;
    int j = 0;
    for (;;) {
        {
            unique_lock<mutex> lock(io);
            cout << "producer " << i << " " << j << endl;
        }
        p.produce(j++);
    }
}

void consume(int i) {
    consumer c;
    for (;;) {
        int j = c.consume();
        {
            unique_lock<mutex> lock(io);
            cout << "consumer " << i << " " << j << endl;
        }
    }
}

int main() {
    for (int i = 0; i < m; i++) {
        thread t(produce, i);
        t.detach();
    }
    for (int i = 0; i < n; i++) {
        thread t(consume, i);
        t.detach();
    }

    for (;;)
        ;
}
