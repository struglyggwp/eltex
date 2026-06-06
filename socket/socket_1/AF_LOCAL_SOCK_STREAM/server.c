#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>

#define PATH "/tmp/local_socket.sock"
#define BUFFER_SIZE 1024

int main() {
   
    int server_fd, client_fd;

    char buffer[BUFFER_SIZE] = {0};
    struct sockaddr_un server_addr;

    if ((server_fd = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1){
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    unlink(PATH);

    memset(&server_addr, 0, sizeof(server_addr));
    
    char *message = "hi";

    server_addr.sun_family = AF_LOCAL;

    strncpy(server_addr.sun_path, PATH, sizeof(server_addr.sun_path) - 1);

    socklen_t server_addr_len = sizeof(server_addr);

    if (bind(server_fd, (struct sockaddr*) &server_addr, server_addr_len) == -1){

        perror("bind error");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 4) == -1){

        perror("listen error");
        exit(EXIT_FAILURE);
    }


    printf("server open tcp\n");


    if((client_fd = accept(server_fd, NULL, NULL)) == -1){

        perror("accept exit");
        exit(EXIT_FAILURE);
    }

    ssize_t bites_read = read(client_fd, buffer, BUFFER_SIZE - 1);

    if (bites_read > 0){


        buffer[bites_read] = '\0';

        printf("tcp message:%s\n", buffer);

        if (write(client_fd, message, strlen(message)) == -1){

            perror("write error");
            exit(EXIT_FAILURE);
        }

        printf("message send to client\n");
    }

    close(client_fd);
    close(server_fd);
    unlink(PATH);

    return 0;
}