#ifndef LIPH_PROCESS_H_
#define LIPH_PROCESS_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace liph {

static inline void err_exit(const char *str) {
    perror(str);
    exit(EXIT_FAILURE);
}

inline void bzero(void *p, size_t n) { memset(p, 0, n); }

/* Bit-mask values for 'flags' argument of become_daemon() */
#define BD_NO_CHDIR          1    /* Don't chdir("/") */
#define BD_NO_CLOSE_FILES    2    /* Don't close all open files */
#define BD_NO_REOPEN_STD_FDS 4    /* Don't reopen stdin, stdout, and stderr to /dev/null */
#define BD_NO_UMASK0         8    /* Don't do a umask(0) */
#define BD_MAX_CLOSE         8192 /* Maximum file descriptors to close if sysconf(_SC_OPEN_MAX) is indeterminate */
// Become background process, returns 0 on success, -1 on error
int become_daemon(int flags);

}  // namespace liph

#endif  // LIPH_PROCESS_H_
