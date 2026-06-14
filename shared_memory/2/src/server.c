#include "server.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

static volatile sig_atomic_t server_should_exit = 0;

static void handle_signal(int signal)
{
    (void)signal;
    server_should_exit = 1;
}

static void copy_text(char *dst, size_t dst_size, const char *src)
{
    if (dst_size == 0) {
        return;
    }

    if (src == NULL) {
        dst[0] = '\0';
        return;
    }

    strncpy(dst, src, dst_size - 1);
    dst[dst_size - 1] = '\0';
}

static int find_user(struct shared_chat *chat, long client_id)
{
    for (int i = 0; i < chat->users_count; ++i) {
        if (chat->users[i].active && chat->users[i].client_id == client_id) {
            return i;
        }
    }

    return -1;
}

static int add_user(struct shared_chat *chat, long client_id, const char *username)
{
    if (find_user(chat, client_id) != -1) {
        return 0;
    }

    if (chat->users_count == MAX_USERS) {
        return -1;
    }

    chat->users[chat->users_count].active = 1;
    chat->users[chat->users_count].client_id = client_id;
    copy_text(chat->users[chat->users_count].username,
              sizeof(chat->users[chat->users_count].username), username);
    chat->users_count++;

    return 0;
}

static void delete_user(struct shared_chat *chat, long client_id)
{
    int index = find_user(chat, client_id);

    if (index == -1) {
        return;
    }

    chat->users[index] = chat->users[chat->users_count - 1];
    chat->users_count--;
}

static void add_event(struct shared_chat *chat, enum event_type type,
                      long sender_id, const char *username, const char *text)
{
    struct chat_message msg;

    memset(&msg, 0, sizeof(msg));
    msg.id = chat->next_message_id++;
    msg.type = type;
    msg.sender_id = sender_id;
    copy_text(msg.username, sizeof(msg.username), username);
    copy_text(msg.text, sizeof(msg.text), text);

    if (chat->messages_count == MAX_MESSAGES) {
        memmove(&chat->messages[0], &chat->messages[1],
                sizeof(chat->messages[0]) * (MAX_MESSAGES - 1));
        chat->messages_count--;
    }

    chat->messages[chat->messages_count] = msg;
    chat->messages_count++;
}

static int pop_command(struct shared_chat *chat, struct client_command *cmd)
{
    if (sem_wait(&chat->mutex) == -1) {
        perror("sem_wait mutex");
        return -1;
    }

    if (chat->command_count == 0) {
        sem_post(&chat->mutex);
        return 0;
    }

    *cmd = chat->commands[chat->command_head];
    chat->command_head = (chat->command_head + 1) % MAX_COMMANDS;
    chat->command_count--;

    sem_post(&chat->mutex);
    return 1;
}

static void process_command(struct shared_chat *chat, const struct client_command *cmd)
{
    if (sem_wait(&chat->mutex) == -1) {
        perror("sem_wait mutex");
        return;
    }

    switch (cmd->type) {
        case EVENT_USER_JOINED:
            if (add_user(chat, cmd->sender_id, cmd->username) == -1) {
                fprintf(stderr, "Слишком много клиентов\n");
            } else {
                add_event(chat, EVENT_USER_JOINED, cmd->sender_id, cmd->username, "");
                printf("Пользователь %s присоединился\n", cmd->username);
            }
            break;

        case EVENT_CHAT_MESSAGE:
            add_event(chat, EVENT_CHAT_MESSAGE, cmd->sender_id, cmd->username, cmd->text);
            printf("%s: %s\n", cmd->username, cmd->text);
            break;

        case EVENT_USER_LEFT:
            delete_user(chat, cmd->sender_id);
            add_event(chat, EVENT_USER_LEFT, cmd->sender_id, cmd->username, "");
            printf("Пользователь %s покинул чат\n", cmd->username);
            break;

        default:
            break;
    }

    sem_post(&chat->mutex);
}

int main(void)
{
    struct shared_chat *chat = chat_open_server();

    if (chat == NULL) {
        return EXIT_FAILURE;
    }

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("Сервер чата запущен\n");

    while (!server_should_exit) {
        if (sem_wait(&chat->command_sem) == -1) {
            if (errno == EINTR && server_should_exit) {
                break;
            }
            perror("sem_wait command_sem");
            break;
        }

        while (1) {
            struct client_command cmd;
            int ret = pop_command(chat, &cmd);

            if (ret <= 0) {
                break;
            }

            process_command(chat, &cmd);
        }
    }

    sem_wait(&chat->mutex);
    chat->server_running = 0;
    sem_post(&chat->mutex);

    sem_destroy(&chat->command_sem);
    sem_destroy(&chat->mutex);
    chat_close(chat);
    shm_unlink(SHM_NAME);

    printf("Сервер чата остановлен\n");
    return EXIT_SUCCESS;
}
