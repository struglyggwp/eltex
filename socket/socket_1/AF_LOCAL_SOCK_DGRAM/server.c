#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/local_server_socket.sock"
#define BUFFER_SIZE 1024

int main() {
   
    int udp_fd;

    struct sockaddr_un server_addr, client_addr;

    char buffer[BUFFER_SIZE] = {0};
    char *message = "hello";

    if((udp_fd = socket(AF_LOCAL, SOCK_DGRAM, 0)) == -1){
        
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    socklen_t server_addr_len = sizeof(server_addr);
    socklen_t client_addr_len = sizeof(client_addr);

    unlink(SERVER_PATH);

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sun_family = AF_LOCAL;

    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path));

    if(bind(udp_fd, (struct sockaddr*) &server_addr, server_addr_len) == -1){

        perror("bind error");
        exit(EXIT_FAILURE);
    }

    printf("server udp run\n");

    ssize_t bytes_read = recvfrom(udp_fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*) &client_addr, &client_addr_len);

    if(bytes_read > 0){
        buffer[bytes_read] = '\0';
        printf("message udp:%s\n", buffer);

        if(sendto(udp_fd, message, strlen(message), 0, (struct sockaddr*) &client_addr, client_addr_len) == -1){
            perror("send error");
            exit(EXIT_FAILURE);
        }

        printf("message send to client\n");

    }


    close(udp_fd);
    unlink(SERVER_PATH);

    return 0;
}