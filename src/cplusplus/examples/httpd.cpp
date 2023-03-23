#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "liph/liph.h"
using namespace std;
using namespace liph;

int main(int argc, char **argv) {
    int port = 80;
    int opt;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
        case 'p':
            port = atoi(optarg);
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " [-p port]" << std::endl;
            exit(1);
        }
    }

    singleton<logger>::instance("std.log")->start();
    logging("logger started");

    /* if (become_daemon(0) == -1) { */
    /*     err_exit("become daemon fail"); */
    /* } */

    http_server svr(port);
    logging("server started, port:", port);
    svr.start();

    singleton<logger>::instance("std.log")->stop();
    return 0;
}
