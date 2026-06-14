#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>

#include "common.h"

struct posix_shared_data {
    sem_t server_to_client;
    sem_t client_to_server;
    char message[MESSAGE_SIZE];
};

int main(void)
{
    int fd = shm_open(POSIX_SHM_NAME, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open; start POSIX server first");
        return EXIT_FAILURE;
    }

    struct posix_shared_data *shm = mmap(NULL,
                                        sizeof(struct posix_shared_data),
                                        PROT_READ | PROT_WRITE,
                                        MAP_SHARED,
                                        fd,
                                        0);
    if (shm == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return EXIT_FAILURE;
    }

    sem_wait(&shm->server_to_client);

    printf("Client received: %s\n", shm->message);

    snprintf(shm->message, MESSAGE_SIZE, "Hello");
    printf("Client wrote: %s\n", shm->message);

    sem_post(&shm->client_to_server);

    munmap(shm, sizeof(struct posix_shared_data));
    close(fd);

    return EXIT_SUCCESS;
}
