#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
   
    int udp_fd;
    
    if ((udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){

        perror("socket error");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) == -1){

        perror("inet pton error");
        exit(EXIT_FAILURE);
    }

    socklen_t server_len = sizeof(server);

    char buffer[BUFFER_SIZE] = {0};

    char *message = "hello!";

    if(sendto(udp_fd, message, strlen(message), 0, (struct sockaddr*) &server, sizeof(server)) == -1){

        perror("sendto error");
        exit(EXIT_FAILURE);
    }

    printf("message send to server\n");

    recvfrom(udp_fd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*) &server, &server_len);

    buffer[BUFFER_SIZE - 1] = '\0';

    printf("udp message:%s\n", buffer);



    return 0;
}