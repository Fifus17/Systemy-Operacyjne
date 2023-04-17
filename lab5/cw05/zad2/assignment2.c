#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

double f(double x) {
    return 4/(x*x+1);
}

double field(double a, double b) {
    return (b-a)*(f(a)+f(b))/2;
}

double calculate(int i, double dx, double n) {
    double sum = 0;
    double start = (double) i/n;
    double current = (double) i/n;

    while (current + dx < start + (double) 1/n) {
        sum += field(current, current + dx);
        current += dx;
    }
    return sum + field(current, start + (double) 1/n);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Wrong number of arguments");
        return 1;
    }
    double dx = strtod(argv[1], NULL); // width of a single rectangle
    double n = atoi(argv[2]); // number of processes

    int fd[atoi(argv[2])][2];
    double sum = 0.0;
    double partial_sum;

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    for (int i = 0; i < n; i++) {
        pipe(fd[i]);
        pid_t pid = fork();
        if (pid == 0) {
            close(fd[i][0]);
            double result = calculate(i, dx, n);
            write(fd[i][1], &result, sizeof(double));
            return 0;
        }
        close(fd[i][1]);
    }

     wait(NULL);
    for (int i = 0; i < n; i++) {
        read(fd[i][0], &partial_sum, sizeof(double));
        sum += partial_sum;
    }

    printf("Result of the integral: %f\n", sum);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("Time: %f\n", (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1000000000.0);
    return 0;
}