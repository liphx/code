#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

void usage() { fprintf(stderr, "usage: dirent [directory]\n"); }

int main(int argc, char *argv[]) {
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
