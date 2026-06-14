#include "client.h"
#include "events.h"

#include <semaphore.h>
#include <time.h>

static void sleep_short(void)
{
    nanosleep(&(struct timespec){.tv_nsec = 10000000}, NULL);
}

void *receive(void *arg)
{
    struct client_context *ctx = arg;
    uint64_t last_seen_id = 0;

    if (sem_wait(&ctx->chat->mutex) == 0) {
        for (int i = 0; i < ctx->chat->users_count; ++i) {
            events_push(EVENT_USER_JOINED, NULL, ctx->chat->users[i].username);
        }

        if (ctx->chat->next_message_id > 0) {
            last_seen_id = ctx->chat->next_message_id - 1;
        }

        sem_post(&ctx->chat->mutex);
    }

    while (!ctx->should_exit) {
        struct chat_message local[MAX_MESSAGES];
        int local_count = 0;

        if (sem_wait(&ctx->chat->mutex) == -1) {
            break;
        }

        for (int i = 0; i < ctx->chat->messages_count; ++i) {
            if (ctx->chat->messages[i].id > last_seen_id && local_count < MAX_MESSAGES) {
                local[local_count++] = ctx->chat->messages[i];
            }
        }

        sem_post(&ctx->chat->mutex);

        for (int i = 0; i < local_count; ++i) {
            last_seen_id = local[i].id;

            switch (local[i].type) {
                case EVENT_CHAT_MESSAGE:
                    events_push(EVENT_CHAT_MESSAGE, local[i].text, local[i].username);
                    break;
                case EVENT_USER_JOINED:
                    events_push(EVENT_USER_JOINED, NULL, local[i].username);
                    break;
                case EVENT_USER_LEFT:
                    events_push(EVENT_USER_LEFT, NULL, local[i].username);
                    break;
                default:
                    break;
            }
        }

        sleep_short();
    }

    return NULL;
}
