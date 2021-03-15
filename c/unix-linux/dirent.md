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

```c
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

void usage()
{
    fprintf(stderr, "usage: dirent [directory]\n");
}

int main(int argc, char *argv[])
{
    if (argc > 2) {
        usage();
        exit(1);
    }

    char *pathname = ".";
    if (argc == 2) {
        pathname = argv[1];
    }

    DIR *dp = opendir(pathname);
    if (dp == NULL) {
        perror("opendir fail");
        exit(1);
    }

    struct dirent *dirp;
    int i = 1;
    while ((dirp = readdir(dp)) != NULL) {
        printf("%d\t", i++);
        printf("d_ino = %ld\t", dirp->d_ino);
        printf("d_off = %ld\t", dirp->d_off);
        printf("d_reclen = %d\t", dirp->d_reclen);
        printf("d_type = %d\t", dirp->d_type);
        printf("d_name = %s\n", dirp->d_name);
    }

    closedir(dp);
    exit(0);
}
```