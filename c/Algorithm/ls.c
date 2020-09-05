#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main()
{
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(".")) == NULL) {
        fprintf(stderr, "opendir fail.\n");
        exit(1);
    }

    while ((dirp = readdir(dp)) != NULL)
        printf("%s\t", dirp->d_name);
        
    printf("\n");
    
    if (closedir(dp) != 0){
        fprintf(stderr, "closedir fail.\n");
        exit(1);
    }

    return 0;
}
