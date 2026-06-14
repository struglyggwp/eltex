#include "client.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>

void sender(char *message, struct client_context *ctx)
{
    struct client_command cmd;

    memset(&cmd, 0, sizeof(cmd));
    cmd.type = EVENT_CHAT_MESSAGE;
    cmd.sender_id = ctx->client_id;

    strncpy(cmd.text, message, MAX_MESSAGE_LEN - 1);
    cmd.text[MAX_MESSAGE_LEN - 1] = '\0';

    strncpy(cmd.username, ctx->username, MAX_NAME_LEN - 1);
    cmd.username[MAX_NAME_LEN - 1] = '\0';

    if (chat_send_command(ctx->chat, &cmd) == -1) {
        perror("chat_send_command");
        pthread_mutex_lock(&ctx->mutex);
        ctx->should_exit = 1;
        pthread_mutex_unlock(&ctx->mutex);
    }
}
