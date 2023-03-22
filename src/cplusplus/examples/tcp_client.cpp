#include <unistd.h>

#include <iostream>
#include <thread>

#include "tcp.h"
using namespace std;

void oneloop() {
    auto sock = std::make_shared<Socket>(Socket::TCP | Socket::IPV4);
    sock->connect("127.0.0.1", 8000);
    char buf[1024] = "hello, world";
    sock->write(buf, sizeof(buf));
    sock->read(buf, sizeof(buf));
    cout << "res: " << buf << endl;
}

int main(int argc, char *argv[]) {
    for (int i = 0; i < 10; i++) {
        thread t(oneloop);
        t.detach();
    }
    sleep(2);
}
