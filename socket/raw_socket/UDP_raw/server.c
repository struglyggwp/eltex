#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 7777
#define BUFFER_SIZE 512

int main() {
    int fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket error"); exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind error"); exit(EXIT_FAILURE);
    }

    printf("UDP сервер запущен на порту %d...\n", PORT);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes = recvfrom(fd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*)&client_addr, &addr_len);
        if (bytes <= 0) continue;

        printf("[Сервер] Получено: %s\n", buffer);

        // Модифицируем произвольный символ (например, первый)
        if (buffer[0] != '\0') buffer[0] = '*'; 

        printf("[Сервер] Отправляю измененную строку: %s\n", buffer);
        sendto(fd, buffer, bytes, 0, (struct sockaddr*)&client_addr, addr_len);
    }
    close(fd);
    return 0;
}