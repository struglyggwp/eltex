#ifndef COMMON_H
#define COMMON_H

#include <semaphore.h>
#include <stdint.h>

#define SHM_NAME "/eltex_chat_shm"

#define MAX_MESSAGES 128
#define MAX_USERS 64
#define MAX_MESSAGE_LEN 256
#define MAX_NAME_LEN 64
#define MAX_INPUT_LEN 256
#define MAX_COMMANDS 64

enum event_type {
    EVENT_CHAT_MESSAGE,
    EVENT_USER_JOINED,
    EVENT_USER_LEFT,
    EVENT_SYSTEM_MESSAGE
};

struct chat_message {
    uint64_t id;
    long sender_id;
    enum event_type type;
    char text[MAX_MESSAGE_LEN];
    char username[MAX_NAME_LEN];
};

struct chat_user {
    int active;
    long client_id;
    char username[MAX_NAME_LEN];
};

struct client_command {
    enum event_type type;
    long sender_id;
    char text[MAX_MESSAGE_LEN];
    char username[MAX_NAME_LEN];
};

struct shared_chat {
    int initialized;
    int server_running;

    sem_t mutex;
    sem_t command_sem;

    uint64_t next_message_id;

    int users_count;
    struct chat_user users[MAX_USERS];

    int messages_count;
    struct chat_message messages[MAX_MESSAGES];

    int command_head;
    int command_tail;
    int command_count;
    struct client_command commands[MAX_COMMANDS];
};

struct shared_chat *chat_open_server(void);
struct shared_chat *chat_open_client(void);
void chat_close(struct shared_chat *chat);
int chat_send_command(struct shared_chat *chat, const struct client_command *cmd);

#endif
