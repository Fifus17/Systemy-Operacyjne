#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

int depth = 0;
// volatile sig_atomic_t depth = 0;

void siginfo_test(int signal_number, siginfo_t* siginfo, void* context) {
    printf("Signal number: %d\n", siginfo->si_signo);
    printf("PID: %d\n", siginfo->si_pid);
    printf("UID: %d\n", siginfo->si_uid);
    printf("Signal code: %d\n", siginfo->si_code);
    printf("Status: %d\n", siginfo->si_status);
}

void reset_hand_test(int signal_number) {
    printf("SA_RESETHAND HANDLER\n");
}

void no_defer_test(int signal_number) {
    if(depth < 5) {
        char msg[140];
		sprintf(msg, "Raising new signal in handler, because depth = %d < 5\n", depth);
		depth++;
		write(STDOUT_FILENO, msg, strlen(msg));
		raise(SIGUSR1);
		while(depth<5);
    }
}

int main(int argc, char** argv) {

    if (argc != 1) {
        printf("Invalid number of arguments!\n");
        return 1;
    }

    // siginfo
    printf("SA_SIGINFO\n");
    struct sigaction siginfo_action;
    sigemptyset(&siginfo_action.sa_mask);
    siginfo_action.sa_sigaction = siginfo_test;
    siginfo_action.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &siginfo_action, NULL);
    raise(SIGUSR1);

    // no defer
    printf("SA_NODEFER\n");
    struct sigaction no_defer_action;
    sigemptyset(&no_defer_action.sa_mask);
	no_defer_action.sa_handler = no_defer_test;
    no_defer_action.sa_flags = SA_NODEFER;
    sigaction(SIGUSR1, &no_defer_action, NULL);
	raise(SIGUSR1);

    // resethand
    printf("SA_RESETHAND\n");
    struct sigaction reset_hand_action;
    sigemptyset(&reset_hand_action.sa_mask);
    reset_hand_action.sa_handler = reset_hand_test;
    reset_hand_action.sa_flags = SA_RESETHAND;
    sigaction(SIGUSR1, &reset_hand_action, NULL);
    raise(SIGUSR1);
    printf("First raise done\n");
    raise(SIGUSR1);
    printf("Second raise done\n"); // shouldn't print


    return 0;
}