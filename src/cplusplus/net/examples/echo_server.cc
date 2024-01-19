#include <iostream>

#include "net/server.h"

using namespace std;
using namespace liph::net;

int main() {
    tcp_server server(10000);
    cout << "echo_server listen on port: " << server.port() << '\n';
    server.on_open([](std::shared_ptr<tcp_connect> conn) {
        cout << "open, local address is " << conn->localaddr() << " and peer address is " << conn->peeraddr() << "\n";
    });
    server.on_close([](std::shared_ptr<tcp_connect> conn) { cout << "close\n"; });
    server.on_message([](std::shared_ptr<tcp_connect> conn, std::string str) {
        cout << "message: " << str << '\n';
        conn->send("hello " + str);
    });
    server.run();  // run in this thread
}
