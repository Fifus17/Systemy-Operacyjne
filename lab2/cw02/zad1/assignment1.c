#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>

void lib(char toFind, char toChange, char* inputFile, char* outputFile) {
    FILE* input = fopen(inputFile, "r");
    FILE* output = fopen(outputFile, "w");
    char c;
    while ((c = fgetc(input)) != EOF) {
        if (c == toFind) {
            fputc(toChange, output);
        } else {
            fputc(c, output);
        }
    }
    fclose(input);
    fclose(output);
}

void sys(char toFind, char toChange, char* inputFile, char* outputFile) {
    int input = open(inputFile, O_RDONLY);
    int output = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC);
    char c;
    while (read(input, &c, 1) > 0) {
        if (c == toFind) {
            write(output, &toChange, 1);
        } else {
            write(output, &c, 1);
        }
    }
    close(input);
    close(output);
}

int main(int argc, char** argv) {
    if (argc != 5) {
        printf("Wrong number of arguments\n");
        return 1;
    }
    struct timespec timespec_buff_start, timespec_buff_end;
    clock_gettime(CLOCK_REALTIME, &timespec_buff_start);

    lib(argv[1][0], argv[2][0], argv[3], argv[4]);

    clock_gettime(CLOCK_REALTIME, &timespec_buff_end);
    struct timespec diff;
    diff.tv_nsec = timespec_buff_end.tv_nsec - timespec_buff_start.tv_nsec;
    printf("EXECUTION TIME OF C FUNCTIONS: %ldns\n", diff.tv_nsec);

    clock_gettime(CLOCK_REALTIME, &timespec_buff_start);
    sys(argv[1][0], argv[2][0], argv[3], argv[4]);
    clock_gettime(CLOCK_REALTIME, &timespec_buff_end);
    diff.tv_nsec = timespec_buff_end.tv_nsec - timespec_buff_start.tv_nsec;
    printf("EXECUTION TIME OF SYSTEM FUNCTIONS: %ldns\n", diff.tv_nsec);
    return 0;
}