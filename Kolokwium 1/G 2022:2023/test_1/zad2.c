
#include "zad2.h"
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void mask()
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    check_mask();
}

void process()
{
    /*  Stworz nowy process potomny i uruchom w nim program ./check_fork
        W procesie macierzystym:
            1. poczekaj 1 sekundę
            2. wyślij SIGUSR1 do procesu potomnego
            3. poczekaj na zakończenie procesu potomnego */
    
    pid_t child_pid = fork();

    if (child_pid != 0){

        sleep(1);
        kill(child_pid, SIGUSR1);

    } else {

        execl("./check_fork", "./check_fork", NULL);
    }

    wait(NULL);

}

int main()
{
    mask();
    process();

    return 0;
}
