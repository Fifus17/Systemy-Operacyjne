#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <ftw.h>
#include <string.h>

long long totalSize = 0;

int helperFunction(const char *filePath, const struct stat *myStruct, int typeflag) {
    if (typeflag == FTW_F) {
        struct stat stats;
        stat(filePath, &stats);
        totalSize += stats.st_size;
        printf("Name: %s, Size: %lld\n", filePath, stats.st_size);
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Wrong number of arguments\n");
        return 1;
    }

    printf("---------------------------------\n");

    int success = ftw(argv[1], helperFunction, 1);
    if(success != 0) {
        printf("Error occured while traversing the directory tree");
        return 2;
    }

    printf("-------------------\n");
    printf("Total size: %lld\n", totalSize);
    printf("-------------------\n");
    return 0;
}