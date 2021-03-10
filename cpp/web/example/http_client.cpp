#include "../http.h"
#include <iostream>
using namespace http;
using namespace std;

int main(int argc, char *argv[])
{
    http_connection conn("127.0.0.1", 80);
    http_request req;
    req.method("get");
    req.url("/");
    req.host("localhost");
    conn.send_req(req);

    http_response res;
    conn.recv_res(res);
    cout << res.content() << endl;
}
