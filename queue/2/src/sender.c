#include "client.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>



void sender(char *message, struct client_context *ctx)
{
    struct msg_buffer msg;

    msg.mtype = CLIENT_TO_SERVER;
    msg.sender_id = ctx->client_id;
    msg.msg_type_ = EVENT_CHAT_MESSAGE;

    strncpy(msg.mtext, message, MAX_MESSAGE_LEN - 1);
    msg.mtext[MAX_MESSAGE_LEN - 1] = '\0';

    strncpy(msg.username, ctx->username, MAX_NAME_LEN - 1);
    msg.username[MAX_NAME_LEN - 1] = '\0';

    if (msgsnd(ctx->msgid, &msg, sizeof(msg) - sizeof(msg.mtype), 0) == -1) {
        perror("msgsnd");
        pthread_mutex_lock(&ctx->mutex);
        ctx->should_exit = 1;
        pthread_mutex_unlock(&ctx->mutex);
    }
}
