#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    
    int udp_fd;
    
    if((udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){

        perror("socket error");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server, client;

    socklen_t client_len = sizeof(client);

    char buffer[BUFFER_SIZE] = {0};

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if(bind(udp_fd, (struct sockaddr*)&server, sizeof(server)) == -1){

        perror("bind error");
        exit(EXIT_FAILURE);
    }
    printf("server udp open\n");

    char *message = "Hi";

    ssize_t buffer_read = recvfrom(udp_fd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*) &client, &client_len);

    buffer[BUFFER_SIZE - 1] = '\0';

    if (buffer_read > 0){
        printf("udp message:%s\n", buffer);


        sendto(udp_fd, message, strlen(message), 0, (struct sockaddr*) &client, sizeof(client));
        printf("message send to client\n");
    }

    return 0;
}
