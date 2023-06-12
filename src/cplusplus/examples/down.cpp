#include "liph/liph.h"

int main(int argc, char **argv) {
    if (argc < 3) return 1;
    std::ofstream file("tmp");
    auto sock = std::make_shared<liph::net::socket>(liph::net::socket::TCP | liph::net::socket::IPV4);
    sock->connect(argv[1], std::stoi(argv[2]));
    char buffer[1024];
    while (true) {
        int size = sock->read(buffer, sizeof(buffer));
        if (size <= 0) break;
        file.write(buffer, size);
    }
}
