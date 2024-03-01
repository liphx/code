#ifndef LIPH_PROCESS_H_
#define LIPH_PROCESS_H_

#include <sys/file.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "liph/format.h"
#include "liph/macros.h"

namespace liph {

inline void err_exit(const char *str = nullptr) {
    if (str) perror(str);

    exit(EXIT_FAILURE);
}

inline void err_exit(const std::string& str) { err_exit(str.c_str()); }

inline void err_exit_if(bool error) {
    if (error) exit(EXIT_FAILURE);
}

inline void bzero(void *p, size_t n) { memset(p, 0, n); }

// Bit-mask values for 'flags' argument of become_daemon()
#define BD_NO_CHDIR 1           // Don't chdir("/")
#define BD_NO_CLOSE_FILES 2     // Don't close all open files
#define BD_NO_REOPEN_STD_FDS 4  // Don't reopen stdin, stdout, and stderr to /dev/null
#define BD_NO_UMASK0 8          // Don't do a umask(0)
#define BD_MAX_CLOSE 8192       // Maximum file descriptors to close if sysconf(_SC_OPEN_MAX) is indeterminate
// Become background process, returns 0 on success, -1 on error
int become_daemon(int flags);

bool single_proc(const char *lock_file_path, bool exit) {
    bool fail = !lock_file_path;
    if (lock_file_path) {
        int fd = open(lock_file_path, O_RDWR | O_CREAT, 0664);
        if (fd == -1) {
            fail = true;
            std::cerr << format("open {} fail: {}\n", lock_file_path, std::strerror(errno));
        } else if (flock(fd, LOCK_EX | LOCK_NB) != 0) {
            fail = true;
            std::cerr << "another proc lock the file\n";
        } else {
            return true;
        }
    }
    if (fail && exit) ::exit(EXIT_FAILURE);
    return false;
}

}  // namespace liph

#endif  // LIPH_PROCESS_H_
