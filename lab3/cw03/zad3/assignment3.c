#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>

int main(int argc, char** argv) {
    bool isParent = true;
    
    if(argc == 3) {
        isParent = true;
    }
    else if(argc == 4) {
        isParent = argv[3];
    }
    else {
        printf("Invalid number of arguments\n");
        return 1;
    }

    if(!isParent){
        return 0;
    }

    size_t patternSize = strlen(argv[2]);

    if(patternSize > 255) {
        printf("Given pattern is too long!\n");
        return 2;
    }

    struct stat stats;
    char* potentialError;
    char mainProgramPath[2048] = "";
    char givenPath[2048] = "";

    potentialError = realpath(argv[0], mainProgramPath);

    if(potentialError == NULL && mainProgramPath[0] != '/') {
        printf("Error resolving main program path\n");
        return 3;
    }

    potentialError = realpath(argv[1], givenPath);

    if(potentialError == NULL && givenPath[0] != '/') {
        printf("Error resolving given path\n");
        return 4;
    }

    int potentialErrorInt = 0;

    potentialErrorInt = stat(givenPath, &stats);

    if(potentialErrorInt == -1) {
        printf("Error occured while running stat function\n");
        return 5;
    }

    // printf("%s || ", givenPath);

    // Checking for directory
    if(S_ISDIR(stats.st_mode)) {
        DIR* dir = opendir(givenPath);
        // printf("%s ||| ", givenPath);

        if(dir == NULL) {
            printf("Problem with reading given directory\n");
            return 6;
        }

        struct dirent* direntry;

        while((direntry = readdir(dir))) {

            // Checking basic cases
            if(strcmp(direntry->d_name, ".") == 0) continue;
            if(strcmp(direntry->d_name, "..") == 0) continue;

            char entryPath[2048] = "";

            // printf("%s ||| ", entryPath);
            strcpy(entryPath, givenPath);
            strcat(entryPath, "/");
            strcat(entryPath, direntry->d_name);
            // printf("%s ||| ", entryPath);

            int forkResult = fork();

            if(forkResult == 0) {
                if (argc == 4)
                    execl(mainProgramPath, mainProgramPath, entryPath, argv[2], false, NULL);
                else
                    execl(mainProgramPath, mainProgramPath, entryPath, argv[2], NULL);
            }
        }
    }

    // File
    else {
        FILE* openedFile = fopen(givenPath, "r");
        char buffer[patternSize+1];
        size_t size = fread(buffer, sizeof(char), patternSize, openedFile);
        buffer[size] = 0;

        if(strcmp(buffer, argv[2]) == 0) {
            printf("%s %d\n", givenPath, getpid());
            printf("--------------------------------------\n");
            fflush(NULL);
        }
    }

    while (wait(NULL) > 0);


    return 0;
}