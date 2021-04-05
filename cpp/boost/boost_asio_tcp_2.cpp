#include <iostream>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
    if (argc != 2) 
        return 1;
    
    boost::asio::io_context io_context;
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime"); // or "13"
    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    for (;;) {
        char buf[128];
        boost::system::error_code error;
        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        std::cout.write(buf, len);
    }
}
