#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>

int main(int argc, char** argv) {

    if(argc != 2) {
        printf("Invalid number of arguments!\n");
        return 1;
    }

    // getting the path to the directory
    char* dirPath = argv[1];

    // printing border line
    printf("-----------------------------------\n");

    // printing its own name
    printf("My name is: %s, directory name: %s\n", argv[0], dirPath);

    // flushing the buffer
    fflush(stdout);

    // running the ls command
    execl("/bin/ls", "ls", dirPath, NULL);

    return 0;
}