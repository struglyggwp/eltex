#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "common.h"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

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
    int shmid = shmget(SYSV_SHM_KEY, sizeof(struct shared_data), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return EXIT_FAILURE;
    }

    int semid = semget(SYSV_SEM_KEY, SEM_COUNT, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        shmctl(shmid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    union semun arg;
    arg.val = 0;

    if (semctl(semid, SEM_SERVER_TO_CLIENT, SETVAL, arg) == -1) {
        perror("semctl server_to_client");
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    if (semctl(semid, SEM_CLIENT_TO_SERVER, SETVAL, arg) == -1) {
        perror("semctl client_to_server");
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    struct shared_data *shm = shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    snprintf(shm->message, MESSAGE_SIZE, "Hi");
    printf("Server wrote: %s\n", shm->message);

    if (sem_post_num(semid, SEM_SERVER_TO_CLIENT) == -1) {
        perror("semop post");
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    printf("Server waits client answer...\n");

    if (sem_wait_num(semid, SEM_CLIENT_TO_SERVER) == -1) {
        perror("semop wait");
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    printf("Server received: %s\n", shm->message);

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return EXIT_SUCCESS;
}
