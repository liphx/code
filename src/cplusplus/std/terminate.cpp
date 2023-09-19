#include <cassert>
#include <exception>
#include <iostream>

// typedef void (*terminate_handler)();
// std::terminate_handler get_terminate() noexcept;
// std::terminate_handler set_terminate(std::terminate_handler f) noexcept;
// [[noreturn]] void terminate() noexcept;

std::terminate_handler handler;

int main() {
    handler = std::get_terminate();
    auto handler2 = std::set_terminate([]() {
        std::cout << "terminate" << std::endl;
        handler();  // abort
    });
    assert(handler == handler2);
    std::terminate();
}
