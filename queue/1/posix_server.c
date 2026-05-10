#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_QUEUE "/posix_server_queue"
#define CLIENT_QUEUE "/posix_client_queue"
#define MAX_MESSAGES 10
#define MESSAGE_SIZE 128

int main()
{
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = MAX_MESSAGES,
        .mq_msgsize = MESSAGE_SIZE,
        .mq_curmsgs = 0
    };

    mqd_t server_mq = mq_open(SERVER_QUEUE, O_CREAT | O_WRONLY, 0666, &attr);
    if (server_mq == (mqd_t)-1) {
        perror("mq_open server");
        exit(EXIT_FAILURE);
    }

    mqd_t client_mq = mq_open(CLIENT_QUEUE, O_CREAT | O_RDONLY, 0666, &attr);
    if (client_mq == (mqd_t)-1) {
        perror("mq_open client");
        mq_close(server_mq);
        mq_unlink(SERVER_QUEUE);
        exit(EXIT_FAILURE);
    }

    const char in[] = "Hi am POSIX server";
    char out[MESSAGE_SIZE];

    if (mq_send(server_mq, in, strlen(in) + 1, 0) == -1) {
        perror("mq_send");
        mq_close(server_mq);
        mq_close(client_mq);
        mq_unlink(SERVER_QUEUE);
        mq_unlink(CLIENT_QUEUE);
        exit(EXIT_FAILURE);
    }

    if (mq_receive(client_mq, out, sizeof(out), NULL) == -1) {
        perror("mq_receive");
        mq_close(server_mq);
        mq_close(client_mq);
        mq_unlink(SERVER_QUEUE);
        mq_unlink(CLIENT_QUEUE);
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

    if (mq_unlink(SERVER_QUEUE) == -1) {
        perror("mq_unlink");
        exit(EXIT_FAILURE);
    }

    if (mq_unlink(CLIENT_QUEUE) == -1) {
        perror("mq_unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}
