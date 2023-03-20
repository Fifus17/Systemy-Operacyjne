#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>

void charReverse(char* inputFile, char* outputFile) {

    FILE* inputFileOpened = fopen(inputFile, "r");
    fseek(inputFileOpened, 0, SEEK_END);
    size_t fileSize = ftell(inputFileOpened);
    fseek(inputFileOpened, 0, SEEK_SET);

    char* content = calloc(fileSize, sizeof(char));
    for (int i=0; i<fileSize; i++) {
        content[fileSize-i-1] = fgetc(inputFileOpened);
    }

    FILE* outputFileOpened = fopen(outputFile, "w");
    for (int i = 0; i < fileSize; i++)
    {
        fputc(content[i], outputFileOpened);
    }
    free(content);
}

void blockReverse(char* inputFile, char* outputFile) {

    FILE* inputFileOpened = fopen(inputFile, "r");
    fseek(inputFileOpened, 0, SEEK_END);
    size_t size = ftell(inputFileOpened);
    fseek(inputFileOpened, 0, SEEK_SET);
    int blockSize = 1024;
    int noOfBlocks = size/blockSize;
    int lastBlockSize = size%blockSize;

    char* content = calloc(blockSize, sizeof(char));
    FILE* outputFileOpened = fopen(outputFile, "w");

    fseek(inputFileOpened, noOfBlocks*blockSize, SEEK_SET);
    fread(content, 1, lastBlockSize, inputFileOpened);
    for (int i = 0; i < lastBlockSize; i++)
    {
        fputc(content[lastBlockSize-i-1], outputFileOpened);
    }

    for (int i=0; i<noOfBlocks; i++) {
        fseek(inputFileOpened, (noOfBlocks-i-1)*blockSize, SEEK_SET);
        fread(content, 1, blockSize, inputFileOpened);
        for (int j = 0; j < blockSize; j++)
        {
            fputc(content[blockSize-j-1], outputFileOpened);
        }
    }    
    free(content);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Wrong number of arguments\n");
        return 1;
    }
    struct timespec timespec_buff_start, timespec_buff_end;
    clock_gettime(CLOCK_REALTIME, &timespec_buff_start);

    charReverse(argv[1], argv[2]);

    clock_gettime(CLOCK_REALTIME, &timespec_buff_end);
    struct timespec diff;
    diff.tv_nsec = timespec_buff_end.tv_nsec - timespec_buff_start.tv_nsec;
    printf("EXECUTION TIME OF READING ONE CHAR AT A TIME: %ldns\n", diff.tv_nsec);

    clock_gettime(CLOCK_REALTIME, &timespec_buff_start);
    blockReverse(argv[1], argv[2]);
    clock_gettime(CLOCK_REALTIME, &timespec_buff_end);
    diff.tv_nsec = timespec_buff_end.tv_nsec - timespec_buff_start.tv_nsec;
    printf("EXECUTION TIME OF READING BLOCKS: %ldns\n", diff.tv_nsec);
    return 0;
}