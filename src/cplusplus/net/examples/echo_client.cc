#include <iostream>
#include <syncstream>
#include <thread>

#include "net/socket.h"

using namespace std;
using namespace std::literals;
using namespace liph::net;

int main() {
    auto func = [] {
        osyncstream oss(cout);
        socket sock{domain::ipv4, protocol::tcp};
        sock.connect("127.0.0.1", 10000);
        if (!sock.ok()) {
            oss << sock.error() << '\n';
            return;
        }
        sock.address();
        oss << "connect to 127.0.0.1:10000, local addr is " << sock.address() << '\n';
        sock.send("echo_client");
        char buffer[1024];
        auto n = sock.recv(buffer, sizeof(buffer));
        if (n > 0) {
            oss << "recv: " << string(buffer, n) << '\n';
        } else if (!sock.ok()) {
            oss << sock.error() << '\n';
        }
        this_thread::sleep_for(10ms);
    };
    vector<thread> threads(100);
    for (auto& t : threads) t = thread(func);
    for (auto& t : threads) t.join();
}
