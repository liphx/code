#include <iostream>
#include <functional>
#include <boost/asio.hpp>

void print(const boost::system::error_code&, boost::asio::steady_timer* t, int *count)
{
    if (*count < 5) {
        std::cout << *count << std::endl;
        ++(*count);
        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
        t->async_wait(std::bind(print, std::placeholders::_1, t, count));
    }
}

int main(int argc, char *argv[])
{
    boost::asio::io_context io;
    int count = 0;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));
    t.async_wait(std::bind(print, std::placeholders::_1, &t, &count));
    io.run();
    std::cout << "Final count is " << count << std::endl;

    return 0;
}
