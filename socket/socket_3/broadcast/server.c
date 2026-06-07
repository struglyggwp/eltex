#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 7777
#define BROADCAST_ADDR "255.255.255.255"

int main() {
    int sock_fd;
    struct sockaddr_in target_addr;
    char *message = "qwertyu123456";
    int flag = 1;

    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    // Включаем broadcast на сокете
    if (setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) == -1) {
        perror("setsockopt error");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(PORT);
    target_addr.sin_addr.s_addr = inet_addr(BROADCAST_ADDR);

    printf("Broadcast сервер запущен. Отправка на %s:%d...\n", BROADCAST_ADDR, PORT);

    while (1) {
        if (sendto(sock_fd, message, strlen(message), 0, (struct sockaddr*)&target_addr, sizeof(target_addr)) == -1) {
            perror("sendto error");
            break;
        }
        printf("Отправлено по широковещанию: %s\n", message);
        sleep(1);
    }

    close(sock_fd);
    return 0;
}