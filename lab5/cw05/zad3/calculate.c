#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

double f(double x) {
    return 4/(x*x+1);
}

double field(double a, double b) {
    return (b-a)*(f(a)+f(b))/2;
}

double calculateField(double dx, double a, double b) {
    double sum = 0;
    double current = a;

    while (current + dx < b) {
        sum += field(current, current + dx);
        current += dx;
    }
    return sum + field(current, b);
}

int main(int argc, char** argv) {

    if (argc != 4) {
        printf("Wrong number of arguments!\n");
        return 1;
    }

    double dx = strtod(argv[1], NULL);
    double a = strtod(argv[2], NULL);
    double b = strtod(argv[3], NULL);

    double result = calculateField(dx, a, b);

    // printf("%f, %f, %f\n", a, b, result);

    int pipe = open("assignment3PIPE", O_WRONLY);
    write(pipe, &result, sizeof(double));

    return 0;
}