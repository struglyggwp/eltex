#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>

#define SERVER_QUEUE "/posix_server_queue"
#define CLIENT_QUEUE "/posix_client_queue"
#define MESSAGE_SIZE 128

int main()
{
    mqd_t server_mq = mq_open(SERVER_QUEUE, O_RDONLY);
    if (server_mq == (mqd_t)-1) {
        perror("mq_open server");
        exit(EXIT_FAILURE);
    }

    mqd_t client_mq = mq_open(CLIENT_QUEUE, O_WRONLY);
    if (client_mq == (mqd_t)-1) {
        perror("mq_open client");
        mq_close(server_mq);
        exit(EXIT_FAILURE);
    }

    const char in[] = "Hello am POSIX client";
    char out[MESSAGE_SIZE];

    if (mq_receive(server_mq, out, sizeof(out), NULL) == -1) {
        perror("mq_receive");
        mq_close(server_mq);
        mq_close(client_mq);
        exit(EXIT_FAILURE);
    }

    if (mq_send(client_mq, in, strlen(in) + 1, 0) == -1) {
        perror("mq_send");
        mq_close(server_mq);
        mq_close(client_mq);
        exit(EXIT_FAILURE);
    }

    printf("%s\n", out);

    if (mq_close(server_mq) == -1) {
        perror("mq_close");
        exit(EXIT_FAILURE);
    }

    if (mq_close(client_mq) == -1) {
        perror("mq_close");
        exit(EXIT_FAILURE);
    }

    return 0;
}
