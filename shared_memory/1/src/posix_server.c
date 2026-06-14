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
    shm_unlink(POSIX_SHM_NAME);

    int fd = shm_open(POSIX_SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    if (ftruncate(fd, sizeof(struct posix_shared_data)) == -1) {
        perror("ftruncate");
        close(fd);
        shm_unlink(POSIX_SHM_NAME);
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
        shm_unlink(POSIX_SHM_NAME);
        return EXIT_FAILURE;
    }

    if (sem_init(&shm->server_to_client, 1, 0) == -1) {
        perror("sem_init server_to_client");
        munmap(shm, sizeof(struct posix_shared_data));
        close(fd);
        shm_unlink(POSIX_SHM_NAME);
        return EXIT_FAILURE;
    }

    if (sem_init(&shm->client_to_server, 1, 0) == -1) {
        perror("sem_init client_to_server");
        sem_destroy(&shm->server_to_client);
        munmap(shm, sizeof(struct posix_shared_data));
        close(fd);
        shm_unlink(POSIX_SHM_NAME);
        return EXIT_FAILURE;
    }

    snprintf(shm->message, MESSAGE_SIZE, "Hi");
    printf("Server wrote: %s\n", shm->message);

    sem_post(&shm->server_to_client);

    printf("Server waits client answer...\n");
    sem_wait(&shm->client_to_server);

    printf("Server received: %s\n", shm->message);

    sem_destroy(&shm->server_to_client);
    sem_destroy(&shm->client_to_server);

    munmap(shm, sizeof(struct posix_shared_data));
    close(fd);
    shm_unlink(POSIX_SHM_NAME);

    return EXIT_SUCCESS;
}
