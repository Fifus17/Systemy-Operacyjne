#include <semaphore.h>

#define PROJECT_IDENTIFIER getenv("HOME")
#define SEM_QUEUE_FNAME "0"
#define SEM_CHAIRS_FNAME "1"
#define SEM_WORKERS_FNAME "2"
#define SEM_BUFFER_MUTEX_FNAME "3"

int NO_WORKERS = 5; // M variable
int NO_CHAIRS = 3; // N variable
int QUEUE_SIZE = 5; // P variable
int NO_CUSTOMERS = 10;