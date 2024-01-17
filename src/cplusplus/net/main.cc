#include <iostream>

#include "socket.h"

using namespace liph::net;

int main(int argc, char **argv) {
    std::cout << std::boolalpha;
    {
        socket sock{domain::ipv4, protocol::tcp};
        std::cout << sock.ok() << " " << sock.error() << '\n';
        sock.connect("127.0.0.1", 8000);
        std::cout << sock.ok() << " " << sock.error() << '\n';
        socket sock2{std::move(sock)};
        sock.connect("127.0.0.1", 8000);
        std::cout << sock.ok() << " " << sock.error() << '\n';
        sock2.connect("127.0.0.1", 8000);
        std::cout << sock2.ok() << " " << sock2.error() << '\n';
    }

    {
        socket sock{domain::ipv6, protocol::udp};
        sock.connect("::1", 8000);
        std::cout << sock.ok() << " " << sock.error() << '\n';
    }
    {
        socket sock{domain::ipv6, protocol::udp};
        sock.connect("127.0.0.1", 8000);
        std::cout << sock.ok() << " " << sock.error() << '\n';
    }
}

// Output:
// true
// true
// false Connect by invalid socket
// false Socket is already connected
// true
// false No route to host
