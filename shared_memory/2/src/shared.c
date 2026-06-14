#include "common.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

static struct shared_chat *map_chat(int fd)
{
    void *addr = mmap(NULL, sizeof(struct shared_chat), PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }

    return addr;
}

struct shared_chat *chat_open_server(void)
{
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open server");
        return NULL;
    }

    if (ftruncate(fd, sizeof(struct shared_chat)) == -1) {
        perror("ftruncate");
        close(fd);
        return NULL;
    }

    struct shared_chat *chat = map_chat(fd);
    close(fd);

    if (chat == NULL) {
        return NULL;
    }

    memset(chat, 0, sizeof(*chat));

    if (sem_init(&chat->mutex, 1, 1) == -1) {
        perror("sem_init mutex");
        munmap(chat, sizeof(*chat));
        return NULL;
    }

    if (sem_init(&chat->command_sem, 1, 0) == -1) {
        perror("sem_init command_sem");
        sem_destroy(&chat->mutex);
        munmap(chat, sizeof(*chat));
        return NULL;
    }

    chat->initialized = 1;
    chat->server_running = 1;
    chat->next_message_id = 1;

    return chat;
}

struct shared_chat *chat_open_client(void)
{
    int fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open client");
        return NULL;
    }

    struct shared_chat *chat = map_chat(fd);
    close(fd);

    if (chat == NULL) {
        return NULL;
    }

    if (!chat->initialized || !chat->server_running) {
        fprintf(stderr, "Сервер чата не запущен\n");
        munmap(chat, sizeof(*chat));
        return NULL;
    }

    return chat;
}

void chat_close(struct shared_chat *chat)
{
    if (chat != NULL) {
        munmap(chat, sizeof(*chat));
    }
}

int chat_send_command(struct shared_chat *chat, const struct client_command *cmd)
{
    if (chat == NULL || cmd == NULL) {
        return -1;
    }

    if (sem_wait(&chat->mutex) == -1) {
        perror("sem_wait");
        return -1;
    }

    if (chat->command_count == MAX_COMMANDS) {
        sem_post(&chat->mutex);
        fprintf(stderr, "Очередь команд заполнена\n");
        return -1;
    }

    chat->commands[chat->command_tail] = *cmd;
    chat->command_tail = (chat->command_tail + 1) % MAX_COMMANDS;
    chat->command_count++;

    sem_post(&chat->mutex);
    sem_post(&chat->command_sem);

    return 0;
}
