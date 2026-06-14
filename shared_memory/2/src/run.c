#include "client.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void send_service_event(struct client_context *ctx, enum event_type type)
{
    struct client_command cmd;

    memset(&cmd, 0, sizeof(cmd));
    cmd.type = type;
    cmd.sender_id = ctx->client_id;
    strncpy(cmd.username, ctx->username, MAX_NAME_LEN - 1);
    cmd.username[MAX_NAME_LEN - 1] = '\0';

    if (chat_send_command(ctx->chat, &cmd) == -1) {
        fprintf(stderr, "Ошибка отправки служебного сообщения\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <username>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct shared_chat *chat = chat_open_client();

    if (chat == NULL) {
        return EXIT_FAILURE;
    }

    struct client_context context = {
        .chat = chat,
        .client_id = (long)getpid(),
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .should_exit = 0
    };

    strncpy(context.username, argv[1], MAX_NAME_LEN - 1);
    context.username[MAX_NAME_LEN - 1] = '\0';

    send_service_event(&context, EVENT_USER_JOINED);

    pthread_t receiver_thread;
    pthread_t client_thread;

    if (pthread_create(&receiver_thread, NULL, receive, &context) != 0) {
        perror("pthread_create receiver");
        chat_close(chat);
        return EXIT_FAILURE;
    }

    if (pthread_create(&client_thread, NULL, client_init, &context) != 0) {
        perror("pthread_create client");
        context.should_exit = 1;
        pthread_join(receiver_thread, NULL);
        chat_close(chat);
        return EXIT_FAILURE;
    }

    pthread_join(client_thread, NULL);

    pthread_mutex_lock(&context.mutex);
    context.should_exit = 1;
    pthread_mutex_unlock(&context.mutex);

    send_service_event(&context, EVENT_USER_LEFT);

    pthread_join(receiver_thread, NULL);
    chat_close(chat);

    return EXIT_SUCCESS;
}
