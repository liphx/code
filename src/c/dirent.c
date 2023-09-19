#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

// DIR *opendir (const char *name);
// DIR *fdopendir (int fd);
// success: pointer; error: NULL

// int closedir (DIR *dirp);
// success: 0; error: -1

// struct dirent *readdir(DIR *dirp);
// success: pointer; error or EOF: NULL

// /usr/include/x86_64-linux-gnu/bits/dirent.h
// struct dirent {
//     __ino_t d_ino;
//     __off_t d_off;
//     unsigned short int d_reclen;
//     unsigned char d_type;
//     char d_name[256];
// };

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "usage: dirent [directory]\n");
        exit(1);
    }
    char *pathname = argc == 2 ? argv[1] : ".";
    DIR *dp = opendir(pathname);
    if (dp == NULL) {
        perror("opendir fail");
        exit(1);
    }
    struct dirent *dirp;
    int i = 1;
    while ((dirp = readdir(dp)) != NULL) {
        printf("%d\t", i++);
        // printf("d_ino = %ld\t", dirp->d_ino);
        // printf("d_off = %ld\t", dirp->d_off);
        printf("d_reclen = %d\t", dirp->d_reclen);
        printf("d_type = %d\t", dirp->d_type);
        printf("d_name = %s\n", dirp->d_name);
    }
    closedir(dp);
}
