#include <iostream>
#include <boost/asio.hpp>

int main(int argc, char *argv[])
{
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
    t.wait(); // synchronously, block 5 seconds.
    std::cout << "Hello, world!" << std::endl;
    return 0;
}
