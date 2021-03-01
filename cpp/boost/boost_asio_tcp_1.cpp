#include <iostream>
#include <string>
#include <ctime>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
    boost::asio::io_context io_context;
    // Daytime protocol, port 13, need root privilege
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

    for (;;) {
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        time_t now = time(0);
        std::string msg = ctime(&now);
        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(msg), ignored_error);
    }
}
