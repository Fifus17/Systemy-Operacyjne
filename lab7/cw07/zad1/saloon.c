#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <string.h>
#include <fcntl.h>

int NO_WORKERS = 5; // M variable
int NO_CHAIRS = 3; // N variable
int NO_WAITING_SEATS = 5; // P variable
int NO_CUSTOMERS = 10;

int main(void) {
    key_t key = ftok(getenv("HOME"), 0);
    if (key == -1) return (1);
    int shm_id = shmget(key, 4096, 0644 | IPC_CREAT);
    char *shm;
    shm = shmat(shm_id, NULL, 0);
    shm[0] = '\0';

    
}