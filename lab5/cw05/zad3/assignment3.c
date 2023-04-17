#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/wait.h>

int main(int argc, char** argv) {

    if(argc != 3) {
        printf("Wrong number of arguments\n");
        return 1;
    }

    int n = atoi(argv[2]);

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    mkfifo("assignment3PIPE", S_IFIFO | S_IRWXU);

    double section = (double) 1/n;

    for (int i = 0; i < n; i++) {
        if(fork() == 0) {
            char lower_range[128];
            char upper_range[128];
            sprintf(lower_range, "%f", i*section);
            sprintf(upper_range, "%f", (double) (i+1)/n);
            execl("calculate", "calculate", argv[1], lower_range, upper_range, NULL);
            return 0;
        }
    }


    double sum = 0.0;
    double partial_sum;
    int pipe = open("assignment3PIPE", O_RDONLY);


    for (int i = 0; i < n; i++) {
        read(pipe, &partial_sum, sizeof(double));
        sum += partial_sum;
    }
    printf("Result: %f\n", sum);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("Time: %f\n", (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1000000000.0);

    return 0;
}