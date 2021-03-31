#include "http.h"
#include <iostream>
using namespace std;

int main()
{
    http_connection conn("124.70.71.134", 80);
    http_request req;
    req.method("get");
    req.url("/");
    req.host("124.70.71.134");
    conn.send_req(req);

    http_response res;
    conn.recv_res(res);
    cout << res.content() << endl;
}
