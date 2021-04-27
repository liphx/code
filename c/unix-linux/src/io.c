#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

typedef struct {
    int fd;
} FILE;

FILE *fopen(const char *pathname, const char *mode)
{
    if (pathname == NULL || mode == NULL)
        return NULL;
    int flags = 0;
    if (strcmp(mode, "r") == 0)
        flags = O_RDONLY;
    else
        return NULL;
    int fd = open(pathname, flags);
    if (fd < 0)
        return NULL;
    FILE *pf = (FILE *)malloc(sizeof(FILE));
    return pf;
}

int fclose(FILE *stream)
{
    if (stream == NULL)
        return 0;
    return close(stream->fd);
}

int main(int argc, char *argv[])
{

}
