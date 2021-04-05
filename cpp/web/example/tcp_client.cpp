#include "tcp.h"
#include <iostream>
using namespace std;

int main()
{
    tcp_connection conn("127.0.0.1", 8000);
    char msg[] = "hello, world";
    conn.writen(msg, sizeof(msg));
    char res[BUFSIZ];
    conn.read(res, BUFSIZ);
    cout << "res: " << res << endl;
}
