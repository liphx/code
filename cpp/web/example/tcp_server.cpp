#include "tcp.h"
#include "common.hpp"
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

int main()
{
    tcp_server server(8000);
    server.start([](tcp_connection& client) {
        auto& req = client.read_buf_;
        cout << "req: " << req.data() << endl;
        auto& res = client.write_buf_;
        res.insert(res.end(), req.begin(), req.end());
    });
}
