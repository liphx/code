#include <stdio.h>

#include "liph.h"

void test_starts_with() {
    if (starts_with("abc", "a")) {
        puts("yes");
    }
}

int main(int argc, char **argv) { test_starts_with(); }
