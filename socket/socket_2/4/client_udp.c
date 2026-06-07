#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 7777
#define BUFFER_SIZE 512

int main() {
   
    char buffer[BUFFER_SIZE] = {0};
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1){
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char *message = "i need time";

    if(sendto(fd, message , strlen(message), 0, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1){
        perror("sendto error");
        exit(EXIT_FAILURE);
    }

    printf("Пакет серверу отправлен\n");


    memset(buffer, 0, BUFFER_SIZE);

    ssize_t bytes_read = recv(fd, buffer, BUFFER_SIZE - 1, 0);

    if(bytes_read < 0){
        perror("recv error");
        exit(EXIT_FAILURE);
    }

    buffer[bytes_read] = '\0';

    printf("Time:%s\n", buffer);

    

    close(fd);


    return 0;
}