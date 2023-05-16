#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <string.h>

#include "variables.h"
#include "shared_memory.h"
#include "sem.h"

// #define BARBER_EXEC "./worker"
// #define CLIENT_EXEC "./client"

#define NO_WORKERS 3      // M
#define NO_CHAIRS 2       // N
#define QUEUE_SIZE 5        // P
#define NO_CUSTOMERS 6

static int sem_queue;
static int sem_chairs;
static int sem_barbers;
static int buffer_mutex;

void close_semaphores(void);
void unlink_semaphores(void);
void create_semaphores(void);

int main(void) {
    char *shared = attach_shared_memory(getenv("HOME"), 4096);
    if(shared == NULL) {
        exit(1);
    }
    shared[0] = '\0';

    unlink_semaphores();
    create_semaphores();

    for(int i = 0; i < NO_WORKERS; i++)
        if (fork() == 0)
            execl("./worker", "./worker", NULL);

    for(int i = 0; i < NO_CUSTOMERS; i++)
        if (fork() == 0)
            execl("./client", "./client", NULL);

    while(wait(NULL) > 0);

    if (!destroy_shared_memory(getenv("HOME"))) {
        fprintf(stderr, "[ERROR] Failed to release shared memory.\n");
        exit(1);
    }
    close_semaphores();
    printf("Simulation finished.\n");
    fflush(stdout);
    return 0;
}


void close_semaphores(void) {
    close_semaphore(sem_queue);
    close_semaphore(sem_chairs);
    close_semaphore(sem_barbers);
    close_semaphore(buffer_mutex);
}

void unlink_semaphores(void) {
    unlink_semaphore("0");
    unlink_semaphore("1");
    unlink_semaphore("2");
    unlink_semaphore("3");
}

void create_semaphores(void) {
    sem_queue =  create_semaphore("0", NO_CHAIRS);
    sem_chairs =  create_semaphore("1", 0);
    sem_barbers =  create_semaphore("2", 0);
    buffer_mutex =  create_semaphore("3", 1);
}