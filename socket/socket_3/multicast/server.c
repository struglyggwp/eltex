#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8888
#define MULTICAST_GROUP "224.0.0.1"

int main() {
    int sock_fd;
    struct sockaddr_in group_addr;
    char *message = "qwerttyyyuuu1234656";

    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    memset(&group_addr, 0, sizeof(group_addr));
    group_addr.sin_family = AF_INET;
    group_addr.sin_port = htons(PORT);
    group_addr.sin_addr.s_addr = inet_addr(MULTICAST_GROUP);

    printf("Multicast сервер запущен. Отправка в группу %s:%d...\n", MULTICAST_GROUP, PORT);

    while (1) {
        if (sendto(sock_fd, message, strlen(message), 0, (struct sockaddr*)&group_addr, sizeof(group_addr)) == -1) {
            perror("sendto error");
            break;
        }
        printf("Отправлено в группу: %s\n", message);
        sleep(1);
    }

    close(sock_fd);
    return 0;
}