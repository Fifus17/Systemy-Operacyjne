#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

typedef enum {
    ignore,
    mask,
    pending,
    invalid
} type;

int parent = 1;

type parser(char* input) {
    if (strcmp(input, "ignore") == 0) return ignore;
    else if (strcmp(input, "mask") == 0) return mask;
    else if (strcmp(input, "pending") == 0) return pending;
    else return invalid;
}

void type_pending() {
    sigset_t sigset;
    sigpending(&sigset);
    if (sigismember(&sigset, SIGUSR1)) printf("(%d) SIGUSR1 PENDING\n", getpid());
    else printf("(%d) SIGUSR1 NOT PENDING\n", getpid());
}

void block() {
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &action.sa_mask, NULL);
}

void raise_signal() {
    printf("(%d) RAISING SIGNAL\n", getpid());
    raise(SIGUSR1);
}

void type_handler(int signal) { printf("(%d) SIGNAL HANDLED\n", getpid()); }

void run(type given_type) {
    switch (given_type) {
        case ignore:
            signal(SIGUSR1, SIG_IGN);
            raise_signal();
            break;
        case mask:
            signal(SIGUSR1, type_handler);
            if (parent) {
                block();
                raise_signal();
            }
            break;
        case pending:
            signal(SIGUSR1, type_handler);
            if (parent) {
                block();
                raise_signal();
            }
            type_pending();
            break;
        case invalid:
            break;
    }
}

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        printf("Wrong number of arguments\n");
        return 1;
    }

    type given_type = parser(argv[1]);
    if (given_type == invalid) {
        printf("Invalid assignment type\n");
        return 2;
    }
    if (argc == 3) parent = 0;
    run(given_type);
    fflush(NULL);
    
    if(parent) execl(argv[0], argv[0], argv[1], "child", NULL);

    fflush(NULL);
    return 0;
}