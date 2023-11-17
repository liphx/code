#include <unistd.h>

#include <cstdlib>
#include <iostream>

int main() {
    void *addr = sbrk(0);
    std::cout << addr << std::endl;

    addr = malloc(0);
    std::cout << addr << std::endl;
    free(addr);  // avoid memory leak
}
