#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#define PATH getenv("HOME")
#define MAX_NO_CLIENTS 20
#define MAX_MESSAGE_SIZE 1024

typedef enum MessageType {
    INIT,
    LIST,
    ALL,
    ONE,
    STOP
} MessageType;

typedef struct Message {
    key_t queue_key;
    int client_ID;
    int recipient_ID;
    MessageType type;
    char text[MAX_MESSAGE_SIZE];
    struct tm time_struct;
} Message;