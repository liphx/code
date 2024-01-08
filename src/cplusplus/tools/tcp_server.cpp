#include <iostream>

#include "liph/liph.h"
using namespace std;
using namespace liph::net;

int main() {
    tcp_server server(8000);
    server.run([](tcp_connect& conn) {
        auto& req = conn.read_buf_;
        cout << "req: " << req.data() << endl;
        auto& res = conn.write_buf_;
        res.insert(res.end(), req.begin(), req.end());
    });
}