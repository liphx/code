#include "../tcp.h"
using namespace tcp;

int main(int argc, char *argv[])
{
    tcp_server server(9000);
    server.start([](const tcp_connection& client){
        char buf[BUFSIZ];
        int size = client.read(buf, BUFSIZ);
        if (size > 0 && size <= BUFSIZ) {
            client.writen(buf, size);
        }
    });
}
