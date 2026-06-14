#ifndef COMMON_H
#define COMMON_H

#define MESSAGE_SIZE 256

#define POSIX_SHM_NAME "/shm_task1_example"

#define SYSV_SHM_KEY 0x12345
#define SYSV_SEM_KEY 0x54321

#define SEM_SERVER_TO_CLIENT 0
#define SEM_CLIENT_TO_SERVER 1
#define SEM_COUNT 2

struct shared_data {
    char message[MESSAGE_SIZE];
};

#endif
