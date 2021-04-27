#ifndef COMMON_H_
#define COMMON_H_

/* standard library */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

/* os header */
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static inline void err_exit(int err_code, int exit_code)
{
    fprintf(stderr, "errno = %d, %s\n", err_code, strerror(err_code));
    exit(exit_code);
}

static inline void hr()
{
    puts("-------------------------");
}

#endif
