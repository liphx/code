#include <future>
#include <iostream>
#include <stdexcept>

int fn(int n) {
    if (n < 1) throw std::invalid_argument("n < 1");
    if (n < 3) return 1;
    return fn(n - 1) + fn(n - 2);
}

int main() {
    try {
        auto fut = std::async(fn, 20);
        std::cout << std::boolalpha;
        int res = fut.get();
        std::cout << "fn(20) = " << res << std::endl;
        auto fut2 = std::async(fn, -20);
        res = fut2.get();
        std::cout << "fn(-20) = " << res << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    try {
        std::thread t(fn, -20);
        t.join();
    } catch (const std::invalid_argument& e) {  // can not catch
        std::cerr << e.what() << std::endl;
    }
}

template <typename F, typename... Ts>
inline std::future<typename std::result_of<F(Ts...)>::type> reallyAsync(F&& f, Ts&&...params) {
    return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}

// C++14
template <typename F, typename... Ts>
inline auto reallyAsync(F&& f, Ts&&...params) {
    return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}
