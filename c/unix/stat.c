#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

void filetype(mode_t mode, char buf[])
{
    if (S_ISREG(mode))
        strcpy(buf, "regular file");
    else if(S_ISDIR(mode))
        strcpy(buf, "directory file");
    else if(S_ISCHR(mode))
        strcpy(buf, "character special file");
    else if(S_ISBLK(mode))
        strcpy(buf, "block special file");
    else if(S_ISFIFO(mode))
        strcpy(buf, "pipe or FIFO");
    else if(S_ISLNK(mode))
        strcpy(buf, "symbolic link");
    else if(S_ISSOCK(mode))
        strcpy(buf, "socket");
    else
        strcpy(buf, "unknow file type");
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: stat pathname\n");
        exit(1);
    }

    struct stat st;
    if (stat(argv[1], &st) != 0) {
        perror("stat error");
        exit(1);
    }

    printf("file %s stat:\n", argv[1]);

    char typebuf[20];
    filetype(st.st_mode, typebuf);
    printf("    st_mode = %ld, %s\n", st.st_mode, typebuf);

    printf("    st_ino = %ld\n", st.st_ino);
    printf("    st_dev = %ld\n", st.st_dev);
    printf("    st_nlink = %ld\n", st.st_nlink);
    printf("    st_uid = %ld\n", st.st_uid);
    printf("    st_gid = %ld\n", st.st_gid);
    printf("    st_size = %ld\n", st.st_size);
    printf("    st_blocks = %ld\n", st.st_blocks);

    char timebuf[20];

    // 最近访问时间
    (void)strftime(timebuf, sizeof(timebuf), "%F %T", localtime(&st.st_atime));
    printf("    st_atime = %ld, localtime is %s\n", st.st_atime, timebuf);

    // 最近修改时间(文件内容)
    (void)strftime(timebuf, sizeof(timebuf), "%F %T", localtime(&st.st_mtime));
    printf("    st_mtime = %ld, localtime is %s\n", st.st_mtime, timebuf);

    // 最近变化时间(内容，权限，所有者等)
    (void)strftime(timebuf, sizeof(timebuf), "%F %T", localtime(&st.st_ctime));
    printf("    st_ctime = %ld, localtime is %s\n", st.st_ctime, timebuf);

    exit(0);
}
