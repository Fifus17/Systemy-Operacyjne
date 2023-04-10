// Write a program that creates a child process and then in an infinite loop sends him SIGUSR1 signals. Child process should count and write current count of them.

#include <stdio.h>
#include <unistd.h> 
#include <signal.h>
#include <sys/types.h>

int count = 0;

void signal_handler(int signum) {
    count += 1;
    printf("Number of received signals: %i\n", count);
}

int main(int argc, char** argv) {

    pid_t parentPID = getpid();
    pid_t childPID;
    fork();
    printf("My PID: %i\n", getpid());
    // printf("Child PID: %i\n", childPID);
    
    if (!(getpid() == parentPID)) {
        printf("My PID: %i, in handler\n", getpid());
        signal(SIGUSR1, signal_handler);
    }
    else {
        int i = 0;
        printf("My PID: %i, in sender\n", getpid());
        while (i < 1000) {
            i++;
            // kill(parentPID, SIGUSR1);
        }
    }

    return 0;
}