// Write a program that sends a signal SIGUSR1 to itself (raise). With a function (signal) make an uncoming signal print "wow what a signal" in terminal.

#include <stdio.h>
#include <unistd.h> 
#include <signal.h>

void signal_handler(int signum) {
    printf("ja cie ale sygnal\n");
}

int main(int argc, char** argv) {

    signal(SIGUSR1, signal_handler);

    raise(SIGUSR1);

    while (1) {
        sleep(100);
    }

}