#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;

    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    const char *message = "Hi";

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket create");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) == -1){
        perror("error listen");
        exit(EXIT_FAILURE);
    }

    printf("server wait connection with port %d\n", PORT);

    if((client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen)) == -1){
        perror("error accept");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);

    if(bytes_read > 0){
        buffer[bytes_read] = '\0';
        printf("client send message:%s\n", buffer);

        send(client_fd, message, strlen(message), 0);
        printf("message send to client.\n");
    }

    close(server_fd);
    close(client_fd);

    return 0;
}
