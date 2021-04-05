#include "common.hpp"
#include <cstdlib>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>

int set_fl(int fd, int flags)
{
    int prev = fcntl(fd, F_GETFL);
    if (prev == -1) {
        return -1;
    }
    return fcntl(fd, F_SETFL, prev | flags);
}

unsigned int rand_sleep(unsigned int max_second)
{
    unsigned int t;
    std::ifstream random("/dev/random");
    random >> t;
    random.close();
    return sleep(t % max_second);
}
