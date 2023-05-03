#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#include "header.h"

int queue_ID;
int server_queue_ID;
int client_ID;
key_t queue_key;

int init() {
    Message *msg = malloc(sizeof(Message));
    msg->type = INIT;
    time_t msg_time = time(NULL);
    msg->time_struct = *localtime(&msg_time);
    msg->queue_key = queue_key;

    msgsnd(server_queue_ID, msg, sizeof(Message), 0);
    msgrcv(queue_ID, msg, sizeof(Message), 0, 0);

    int client_ID = msg->client_ID;
    if (client_ID == -1) {
        printf("There are no more free spots on the server. Try again later.\n");
        return(-1);
    }
    return client_ID;
}

void list() {
    Message *msg = malloc(sizeof(Message));
    msg->type = LIST;
    time_t msg_time = time(NULL);
    msg->time_struct = *localtime(&msg_time);

    msgsnd(server_queue_ID, msg, sizeof(Message), 0);
    msgrcv(queue_ID, msg, sizeof(Message), 0, 0);
    printf("%s\n", msg->text);
}

void all(char *message_text) {
    Message *msg = malloc(sizeof(Message));
    msg->type = ALL;
    time_t msg_time = time(NULL);
    msg->time_struct = *localtime(&msg_time);
    msg->client_ID = client_ID;
    strcpy(msg->text, message_text);

    msgsnd(server_queue_ID, msg, sizeof(Message), 0);
}

void one(int recipient_ID, char *message_text) {
    Message *msg = malloc(sizeof(Message));
    msg->type = ONE;
    time_t msg_time = time(NULL);
    msg->time_struct = *localtime(&msg_time);
    msg->client_ID = client_ID;
    msg->recipient_ID = recipient_ID;
    strcpy(msg->text, message_text);

    msgsnd(server_queue_ID, msg, sizeof(Message), 0);
}

void stop() {
    Message *msg = malloc(sizeof(Message));
    msg->type = STOP;
    time_t msg_time = time(NULL);
    msg->time_struct = *localtime(&msg_time);
    msg->client_ID = client_ID;

    msgsnd(server_queue_ID, msg, sizeof(Message), 0);
    msgctl(queue_ID, IPC_RMID, NULL);
    printf("Logging out...\n");
    exit(0);
}

void incoming_message() {
    Message *msg = malloc(sizeof(Message));
    while(msgrcv(queue_ID, msg, sizeof(Message), 0, IPC_NOWAIT) >= 0) {
        if(msg->type == STOP) stop();
        else {
            struct tm msg_time = msg->time_struct;
            printf("%02d:%02d:%02d | CLIENT ID: %d >>> %s\n", msg_time.tm_hour, msg_time.tm_min, msg_time.tm_sec, msg->client_ID, msg->text);
        }
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, stop);
    srand(time(NULL));
    queue_key = ftok(PATH, rand() % 255 + 1);
    queue_ID = msgget(queue_key, IPC_CREAT | 0666);
    key_t server_key = ftok(PATH, 0);
    server_queue_ID = msgget(server_key, 0);
    client_ID = init();

    size_t length;
    ssize_t input;
    char *command;

    while(1) {
        printf(">>> ");
        input = getline(&command, &length, stdin);
        command[input - 1] = '\0';
        incoming_message();
        if (length <= 1) continue;

        char *current = strtok(command, " ");
        if (strcmp(current, "INIT") == 0) printf("You are already initialized\n");
        else if (strcmp(current, "LIST") == 0) list();
        else if (strcmp(current, "2ALL") == 0) {
            current = strtok(NULL, " ");
            char *text = current;
            all(text);
        }
        else if (strcmp(current, "2ONE") == 0) {
            current = strtok(NULL, " ");
            int recipient_ID = atoi(current);
            char *text = current;
            one(recipient_ID, text);
        }
        else if (strcmp(current, "STOP") == 0) stop();
        else printf("Wrong command!\n");
    }
}