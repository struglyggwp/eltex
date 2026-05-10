#ifndef CLIENT_H
#define CLIENT_H

#include "common.h"

#include <pthread.h>


struct client_context {
    int msgid;
    long client_id;
    pthread_mutex_t mutex;
    int should_exit;
    char username[MAX_NAME_LEN];
};


void *client_init(void *arg);
void *receive(void *arg);
void sender(char *message, struct client_context *ctx);

#endif
