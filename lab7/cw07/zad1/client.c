#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <string.h>

#include "shared_memory.h"
#include "variables.h"
#include "sem.h"

static int sem_queue;
static int sem_chairs;
static int sem_barbers;
static int buffer_mutex;

void open_semaphores();
char random_haircut();
void queue_push(char *queue, char byte);
bool queue_full(char *queue);

int main(void) {
    srand(time(NULL) + getpid());

    char *queue = attach_shared_memory(getenv("HOME"), 4096);
    if (queue == NULL) {
        fprintf(stderr, "[ERROR] Can't open queue.\n");
        exit(EXIT_FAILURE);
    }

    open_semaphores();

    if (strlen(queue) >= QUEUE_SIZE) {
        printf("\t[CLIENT-%d] Queue full. Leaving.\n", getpid());
        fflush(stdout);
    }
    
    aquire(sem_queue);

    aquire(buffer_mutex);
    char haircut = random_haircut();
    printf("\t[CLIENT-%d] New client with haircut no. %d \n", getpid(), haircut);
    fflush(stdout);
    queue_push(queue, haircut);
    release(buffer_mutex);
    
    release(sem_barbers);
    aquire(sem_chairs);

    printf("\t[CLIENT-%d] Client done.\n", getpid());
    fflush(stdout);

    detach_shared_memory(queue);

    return EXIT_SUCCESS;
}

void open_semaphores()
{
    sem_queue = open_semaphore("0");
    sem_chairs = open_semaphore("1");
    sem_barbers = open_semaphore("2");
    buffer_mutex = open_semaphore("3");
}

char random_haircut() {
    return (char) (rand() % 128);
}

void queue_push(char *queue, char byte) {
    if(queue_full(queue)) {
        fprintf(stderr, "[ERROR] Failed to push to queue. Queue full.\n");
        return;
    }
    int size = strlen(queue);
    queue[size] = byte;
    queue[size + 1] = '\0';
}

bool queue_full(char *queue) {
    return (strlen(queue) + 1 == 4096);
}