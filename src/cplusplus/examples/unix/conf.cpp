#include <unistd.h>
// long sysconf(int name);
// long pathconf(const char *pathname, int name);
// long fpathconf(int fd, int name);
// return -1 and set errno if error

#include <iostream>

#define print_sysconf(name) std::cout << #name ": " << sysconf(name) << std::endl
#define print_pathconf(name) std::cout << #name ": " << pathconf("/", name) << std::endl

int main() {
    print_sysconf(_SC_OPEN_MAX);   // The maximum number of open files per user id.
    print_sysconf(_SC_PAGESIZE);   // The size of a system page in bytes.
    print_pathconf(_PC_NAME_MAX);  // The maximum number of bytes in a file name.
    print_pathconf(_PC_PATH_MAX);  // The maximum number of bytes in a pathname
}
