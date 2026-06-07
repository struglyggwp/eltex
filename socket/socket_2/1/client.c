#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 7777
#define BUFFER_SIZE 512

int main(){

    int client_fd;
    char buffer[BUFFER_SIZE] = {0};

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    socklen_t server_addr_len = sizeof(server_addr);

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(client_fd, (struct sockaddr*) &server_addr, server_addr_len) == -1){
        perror("connect error");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if(bytes_read > 0){

        buffer[bytes_read] = '\0';

        printf("Время:%s\n", buffer);
    }

    close(client_fd);

    return 0;
}