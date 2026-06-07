#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#define PORT 7777


void* service_handle(void *fd){

    printf("Начинаю обслуживание клиента.\n");

    int client_fd = *(int*)fd;
    free(fd);

    pthread_detach(pthread_self());

    time_t raw_time = time(NULL);
    char *time_str = ctime(&raw_time);

    if(send(client_fd, time_str, strlen(time_str), 0) == -1){
        perror("send error");

    }

    close(client_fd);
    printf("Обслуживание клиента завершено.\n");

    return NULL;
}





int main(){

    int server_fd, client_fd;
    int flag = 1;

    struct sockaddr_in server_addr, client_addr;

    socklen_t server_addr_len = sizeof(server_addr);
    socklen_t client_addr_len = sizeof(client_addr);

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if(bind(server_fd, (struct sockaddr*) &server_addr, server_addr_len) == -1){
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 10) == -1){
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен. Ожидаю подключений.\n");

    while(1){

    
        if((client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &client_addr_len)) == -1){
            perror("accept error");
            continue;
        }

        printf("Клиент подключен. Запускаю поток.\n");

        pthread_t pthread_service;
        int *new_fd = malloc(sizeof(int));
        *new_fd = client_fd;

        if(pthread_create(&pthread_service, 0, service_handle, (void*)new_fd) != 0){
            perror("pthread error");
            free(new_fd);
            close(client_fd);
        }
    
    }

    close(server_fd);

}