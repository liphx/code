#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code&)
{
    std::cout << "Hello, world!" << std::endl;
}

int main(int argc, char *argv[])
{
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
    t.async_wait(&print); //  asynchronously, return immediately
    io.run();

    return 0;
}
