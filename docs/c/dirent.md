# dirent

```c
#include <dirent.h>

DIR *opendir (const char *__name);
DIR *fdopendir (int __fd);              // success: pointer; error: NULL

int closedir (DIR *__dirp);             // success: 0; error: -1

// /usr/include/x86_64-linux-gnu/bits/dirent.h
struct dirent {
    __ino_t d_ino;
    __off_t d_off;
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];
};

struct dirent *readdir (DIR *__dirp);   // success: pointer; error or EOF: NULL
```

