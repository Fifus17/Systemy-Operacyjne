#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Wrong number of arguments\n");
        return 1;
    }

    pid_t parentPID = getpid();
    int noOfProcesses = atoi(argv[1]);
    int i=0;
    pid_t childPID = 0;

    if(getpid() == parentPID) printf("--------------------------------\n");

    while(i<=noOfProcesses) {
        if(getpid() == parentPID) {
            childPID = fork();
            wait(&childPID);
        }
        else {
            printf("My PID: %i, Parent PID: %i\n", getpid(), getppid());
            exit(0);
        }
        i++;
    }

    if(getpid() == parentPID) {
        printf("Given argument: %i\n", noOfProcesses);
        printf("--------------------------------\n");
    
    }
}