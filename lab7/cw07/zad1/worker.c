#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#include "variables.h"
#include "shared_memory.h"
#include "sem.h"

#define HAIRCUT_TIME 1000
#define TIMEOUT 1000000

static int sem_queue;
static int sem_chairs;
static int sem_barbers;
static int buffer_mutex;

void open_semaphores();
char queue_pop(char *queue);
void queue_push(char *queue, char byte);
bool queue_full(char *queue);
bool queue_empty(char *queue);

int main(void)
{
    char *queue = attach_shared_memory(getenv("HOME"), 4096);
    if (queue == NULL)
    {
        fprintf(stderr, "[ERROR] Can't open queue.\n");
        exit(EXIT_FAILURE);
    }
    open_semaphores();

    printf("\t[BARBER-%d] Spawned\n", getpid());
    fflush(stdout);

    while (true)
    {

        aquire(sem_barbers);

        release(buffer_mutex);
        char haircut = queue_pop(queue);
        release(buffer_mutex);

        printf("\t[BARBER-%d] Processing hairuct no. %d\n", getpid(), haircut);
        fflush(stdout);

        usleep(HAIRCUT_TIME);

        printf("\t[BARBER-%d] Done with hairuct no. %d\n", getpid(), haircut);
        fflush(stdout);

        release(sem_chairs);
        release(sem_queue);

        if (queue_empty(queue))
        {
            usleep(TIMEOUT);
            if (queue_empty(queue))
                break;
        }
    }
    printf("\t[BARBER-%d] Queue empty. Closing. \n", getpid());
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

char queue_pop(char *queue) {
    if(queue_empty(queue)) {
        fprintf(stderr, "[ERROR] Failed to pop from queue. Queue empty.\n");
        return '\0';
    }
    char byte = queue[0];
    memcpy(queue, queue + 1, strlen(queue) + 1);
    return byte;
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

bool queue_empty(char *queue) {
    return (strlen(queue) == 0);
}