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
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pron error");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Подключение к серверу...\n");

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect error");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Соединение установлено! Жду данные от сервера...\n");

    int bytes_read = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_read == -1) {
        perror("read error");
    } else if (bytes_read == 0) {
        printf("server closed connect\n");
    } else {
        buffer[bytes_read] = '\0';
        printf("Время: %s", buffer);
    }

    close(sock_fd);
    
    return 0;
}