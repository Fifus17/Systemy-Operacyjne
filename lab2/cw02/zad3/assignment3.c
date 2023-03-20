#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Wrong number of arguments\n");
        return 1;
    }

    DIR* dir = opendir(argv[1]);

    chdir(argv[1]);

    struct dirent* file;
    struct stat fileStat;
    long long totalSize = 0;

    printf("---------------------------------\n");

    while ((file = readdir(dir))) {
        stat(file->d_name, &fileStat);
        if(!S_ISDIR(fileStat.st_mode)) {
            totalSize += fileStat.st_size;
            printf("Name: %s, Size: %lld\n", file->d_name, fileStat.st_size);
        }
    }
    printf("-------------------\n");
    printf("Total size: %lld\n", totalSize);
    printf("-------------------\n");
    return 0;
}