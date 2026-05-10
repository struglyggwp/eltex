#include "client.h"

#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
   

    if (argc < 2) {
    fprintf(stderr, "Usage: %s <username>\n", argv[0]);
    exit(EXIT_FAILURE);
    }


    key_t key = ftok("/tmp/queue_keyfile", 1);

    int msgid = msgget(key, 0);
    if (msgid == -1) {
        perror("msgget run");
        exit(EXIT_FAILURE);
    }

    struct client_context context = {
        .msgid = msgid,
        .client_id = (long)getpid(),
        .should_exit = 0,
        .mutex = PTHREAD_MUTEX_INITIALIZER
    };

    strncpy(context.username, argv[1], MAX_NAME_LEN - 1);
    context.username[MAX_NAME_LEN - 1] = '\0';

    pthread_t reveiver_thread, client_thread;

    struct msg_buffer join_msg = {
    .mtype = CLIENT_TO_SERVER,
    .sender_id = context.client_id,
    .msg_type_ = EVENT_USER_JOINED
    };

    strncpy(join_msg.username, context.username, MAX_NAME_LEN - 1);
    join_msg.username[MAX_NAME_LEN - 1] = '\0';

    if (msgsnd(msgid, &join_msg, sizeof(join_msg) - sizeof(join_msg.mtype), 0) == -1) {
        perror("msgsnd join");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&reveiver_thread, NULL, receive, &context) != 0) {
        perror("pthread_create receiver");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&client_thread, NULL, client_init, &context) != 0) {
        perror("pthread_create client");
        exit(EXIT_FAILURE);
    }

    pthread_join(client_thread, NULL);
    context.should_exit = 1;
    pthread_join(reveiver_thread, NULL);


   struct msg_buffer exit_msg = {
    .mtype = CLIENT_TO_SERVER,
    .sender_id = context.client_id,
    .msg_type_ = EVENT_USER_LEFT
    };

    strncpy(exit_msg.username, context.username, MAX_NAME_LEN - 1);
    exit_msg.username[MAX_NAME_LEN - 1] = '\0';

    if (msgsnd(msgid, &exit_msg, sizeof(exit_msg) - sizeof(exit_msg.mtype), 0) == -1) {
        perror("msgsnd exit");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
