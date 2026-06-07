#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 7777
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in local_addr;
    char buffer[BUFFER_SIZE] = {0};
    int flag = 1;

    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1) {
        perror("setsockopt error");
    }

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(PORT);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

    if (bind(sock_fd, (struct sockaddr*)&local_addr, sizeof(local_addr)) == -1) {
        perror("bind error");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Broadcast клиент ждет данные на порту %d...\n", PORT);

    while (1) {
        ssize_t bytes_read = recvfrom(sock_fd, buffer, BUFFER_SIZE - 1, 0, NULL, NULL);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Принят Broadcast: %s\n", buffer);
        } else if (bytes_read == -1) {
            perror("recvfrom error");
            break;
        }
    }

    close(sock_fd);
    return 0;
}