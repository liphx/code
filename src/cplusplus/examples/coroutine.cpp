#include <coroutine>
#include <iostream>
using namespace std;

#if 0
struct task {
    void operator()() { coro.resume(); }

    // mapping to the language features
    struct promise_type {
        suspend_always initial_suspend() { return {}; }

        // co_return
        suspend_always final_suspend() noexcept { return {}; }

        // co_yield
        suspend_always yield_value(int) { return {}; }

        auto get_return_object() { return task{handle_type::from_promise(*this)}; }

        void return_void() {}
        void unhandled_exception() { exit(1); }
    };

    using handle_type = coroutine_handle<promise_type>;

    // called by get_return_object()
    task(handle_type h) : coro(h) {}

    // here is the coroutine handle
    handle_type coro;
};

template <class T>
struct generator {};

generator<long long> fib() {
    long long a = 0;
    long long b = 1;
    while (a < b) {
        auto next = a + b;
        co_yield next;
        a = b;
        b = next;
    }
    co_return 0;
}

int main() {
    while (int v = fib(); v != 0) {
        cout << v << '\n';
        v = fib();
    }
}
#endif

int main() {}
