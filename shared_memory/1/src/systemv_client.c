#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "common.h"

static int sem_wait_num(int semid, unsigned short sem_num)
{
    struct sembuf op;

    op.sem_num = sem_num;
    op.sem_op = -1;
    op.sem_flg = 0;

    return semop(semid, &op, 1);
}

static int sem_post_num(int semid, unsigned short sem_num)
{
    struct sembuf op;

    op.sem_num = sem_num;
    op.sem_op = 1;
    op.sem_flg = 0;

    return semop(semid, &op, 1);
}

int main(void)
{
    int shmid = shmget(SYSV_SHM_KEY, sizeof(struct shared_data), 0666);
    if (shmid == -1) {
        perror("shmget; start System V server first");
        return EXIT_FAILURE;
    }

    int semid = semget(SYSV_SEM_KEY, SEM_COUNT, 0666);
    if (semid == -1) {
        perror("semget; start System V server first");
        return EXIT_FAILURE;
    }

    struct shared_data *shm = shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat");
        return EXIT_FAILURE;
    }

    if (sem_wait_num(semid, SEM_SERVER_TO_CLIENT) == -1) {
        perror("semop wait");
        shmdt(shm);
        return EXIT_FAILURE;
    }

    printf("Client received: %s\n", shm->message);

    snprintf(shm->message, MESSAGE_SIZE, "Hello");
    printf("Client wrote: %s\n", shm->message);

    if (sem_post_num(semid, SEM_CLIENT_TO_SERVER) == -1) {
        perror("semop post");
        shmdt(shm);
        return EXIT_FAILURE;
    }

    shmdt(shm);

    return EXIT_SUCCESS;
}
