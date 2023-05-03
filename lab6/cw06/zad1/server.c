#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#include "header.h"

key_t clients_queues[MAX_NO_CLIENTS];
int ID_to_assign = 0;
int server_queue;

void server_log(Message *msg) {
    FILE *logs = fopen("logs.txt", "a");
    struct tm msg_time = msg->time_struct;
    fprintf(logs, "%02d:%02d:%02d", msg_time.tm_hour, msg_time.tm_min, msg_time.tm_sec);

    MessageType msg_type = msg->type;

    switch (msg_type)
    {
        case INIT:
            if(msg->client_ID > -1) fprintf(logs, " | CLIENT ID: %d >>> [INIT] Initiliased\n", msg->client_ID);
            else fprintf(logs, " >>> [INIT] No more free spots.\n");
            break;
        case LIST:
            fprintf(logs, " | CLIENT ID: %d >>> [LIST]\n", msg->client_ID);
            break;
        case ALL:
            fprintf(logs, " | CLIENT ID: %d >>> [2ALL] %s\n", msg->client_ID, msg->text);
            break;
        case ONE:
            fprintf(logs, " | CLIENT ID: %d | RECIPIENT ID: %d >>> [2ONE] %s\n", msg->client_ID, msg->recipient_ID, msg->text);
            break;
        case STOP:
            fprintf(logs, " | CLIENT ID: %d >>> [STOP]\n", msg->client_ID);
            break;
    }

    fclose(logs);
}

void init(Message *msg) {
    if (ID_to_assign != MAX_NO_CLIENTS) {
        msg->client_ID = ID_to_assign;
        clients_queues[ID_to_assign] = msg->queue_key;
        ID_to_assign += 1;
    }
    else msg->client_ID = -1;
    int client_queue_ID = msgget(msg->queue_key, 0);
    msgsnd(client_queue_ID, msg, sizeof(Message), 0);
}

void list(int client_ID) {
    Message *msg = malloc(sizeof(Message));
    msg->type = LIST;
    strcpy(msg->text, "Active clients: \n");
    for (int i = 0; i < MAX_NO_CLIENTS; i++) {
        if (clients_queues[i] != -1) {
            sprintf(msg->text + strlen(msg->text), "%d\n", i);
        }
    }
    int client_queue_ID = msgget(clients_queues[client_ID], 0);
    msgsnd(client_queue_ID, msg, sizeof(Message), 0);
}

void all(Message *msg) {
    for (int i = 0; i < MAX_NO_CLIENTS; i++) {
        if(msg->client_ID != i && clients_queues[i] != -1) {
            int recipient_queue_ID = msgget(clients_queues[i], 0);
            msgsnd(recipient_queue_ID, msg, sizeof(Message), 0);
        }
    }
}

void one(Message *msg) {
    int recipient_ID = msg->recipient_ID;
    int recipient_queue_ID = msgget(clients_queues[recipient_ID], 0);
    msgsnd(recipient_queue_ID, msg, sizeof(Message), 0);
}

void stop(int client_ID) {
    clients_queues[client_ID] = -1;
    if (client_ID < ID_to_assign) ID_to_assign = client_ID;
}

void end() {
    Message *msg = malloc(sizeof(Message));
    msg->type = STOP;

    for (int i = 0; i < MAX_NO_CLIENTS; i++) {
        if(clients_queues[i] != -1) {
        int client_queue_ID = msgget(clients_queues[i], 0);
        msgsnd(client_queue_ID, msg, sizeof(Message), 0);
        msgrcv(server_queue, msg, sizeof(Message), STOP, 0);
        }
    }

    msgctl(server_queue, IPC_RMID, NULL);
    exit(0);

}

int main(int argc, char **argv) {
    signal(SIGINT, end);
    key_t queue_key = ftok(PATH, 0);
    server_queue = msgget(queue_key, IPC_CREAT | 0666);

    for (int i = 0; i < MAX_NO_CLIENTS; i++) clients_queues[i] = -1;
    
    Message *msg = malloc(sizeof(Message));
    while (1) {
        msgrcv(server_queue, msg, sizeof(Message), -6, 0);

        switch (msg->type)
        {
            case INIT:
                init(msg);
                server_log(msg);
                break;
            case LIST:
                list(msg->client_ID);
                server_log(msg);
                break;
            case ALL:
                all(msg);
                server_log(msg);
                break;
            case ONE:
                one(msg);
                server_log(msg);
                break;
            case STOP:
                stop(msg->client_ID);
                server_log(msg);
                break;
        }
    }
}