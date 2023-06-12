#include "liph/liph.h"

int main(int argc, char **argv) {
    if (argc < 2) return 1;
    std::ifstream file(argv[1]);
    liph::net::tcp_server svr;
    std::cout << "port is " << svr.port() << std::endl;
    svr.run([&file](liph::net::tcp_connect& conn) {
        std::cout << "begin conn" << std::endl;
        char buffer[1024];
        while (file.read(buffer, sizeof(buffer))) {
            conn.write_buf_.insert(conn.write_buf_.end(), std::begin(buffer), std::end(buffer));
        }
        std::cout << "end conn" << std::endl;
    });
}
