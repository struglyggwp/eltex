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
   
    int server_fd;

    struct sockaddr_un server_addr;

    socklen_t server_addr_len = sizeof(server_addr);

    char buffer[BUFFER_SIZE]  = {0};

    char *message = "hello";

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sun_family = AF_LOCAL;

    strncpy(server_addr.sun_path, PATH, sizeof(server_addr.sun_path) - 1);

    if ((server_fd = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1){

        perror("socket error");
        exit(EXIT_FAILURE);
    }

    if (connect(server_fd, (struct sockaddr*) &server_addr, server_addr_len) == -1){

        perror("connect error");
        exit(EXIT_FAILURE);
    }

    printf("connected to server tcp\n");

    if (write(server_fd, message, strlen(message)) == -1){

        perror("write error");
        exit(EXIT_FAILURE);
    }

    printf("message send to server\n");

    ssize_t bytes_read = read(server_fd, buffer, BUFFER_SIZE - 1);

    if (bytes_read > 0){

        buffer[bytes_read] = '\0';
        printf("message tcp:%s\n", buffer);
    } else if (bytes_read == -1){
        perror("read error");
        exit(EXIT_FAILURE);
    } else if (bytes_read == 0){
        printf(" server closed connection\n");

    }

    close(server_fd);
    

    return 0;
}