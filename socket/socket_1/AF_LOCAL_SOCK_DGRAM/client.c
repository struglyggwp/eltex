#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/local_server_socket.sock"
#define CLIENT_PATH "/tmp/local_client_socket.sock"
#define BUFFER_SIZE 1024

int main() {
   
    int udp_fd;

    struct sockaddr_un server_addr, client_addr;

    char buffer[BUFFER_SIZE] = {0};
    char *message = "hi";

    unlink(CLIENT_PATH);

    socklen_t server_addr_len = sizeof(server_addr);
    socklen_t client_addr_len = sizeof(client_addr);

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    client_addr.sun_family = AF_LOCAL;
    strncpy(client_addr.sun_path, CLIENT_PATH, sizeof(client_addr.sun_path));
    
    server_addr.sun_family = AF_LOCAL;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(client_addr.sun_path));


    if((udp_fd = socket(AF_LOCAL, SOCK_DGRAM, 0)) == -1){

        perror("socket error");
        exit(EXIT_FAILURE);
    }

    if(bind(udp_fd, (struct sockaddr*) &client_addr, client_addr_len) == -1){

        perror("bind error");
        exit(EXIT_FAILURE);
    }



    if(sendto(udp_fd, message, strlen(message), 0, (struct sockaddr*) &server_addr, server_addr_len) == -1){

        perror("sendto error");
        exit(EXIT_FAILURE);
    }

    printf("message send to server\n");

    ssize_t bytes_read = recvfrom(udp_fd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*) &server_addr, &server_addr_len);

    if(bytes_read > 0){

        buffer[bytes_read] = '\0';

        printf("message udp:%s\n", buffer);
    }

    close(udp_fd);
    unlink(CLIENT_PATH);

    return 0;
}