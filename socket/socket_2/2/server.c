#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h> 
#include <time.h>

#define PORT 7777
#define BUFFER_SIZE 512
#define POOL_SIZE 5

struct Worker {
    pthread_t tid;   
    int client_fd;     
    int is_free;            //  1 - свободен, 0 - занят
    pthread_mutex_t mutex; 
    pthread_cond_t cond;   
};


void* pthread_handle(void* arg) {
    struct Worker *self = (struct Worker*) arg;

    while(1) {
        pthread_mutex_lock(&self->mutex);

        while(self->is_free == 1) {
            pthread_cond_wait(&self->cond, &self->mutex);
        }

        int fd = self->client_fd;
        pthread_mutex_unlock(&self->mutex);

        printf("[Поток %lu] Взял клиента.\n", (unsigned long)pthread_self());

        time_t raw_time = time(NULL);
        char *time_str = ctime(&raw_time);
        
        sleep(2);

        send(fd, time_str, strlen(time_str), 0);
        close(fd);

        pthread_mutex_lock(&self->mutex);
        self->is_free = 1; 
        self->client_fd = -1;
        pthread_mutex_unlock(&self->mutex);

        printf("[Поток %lu] Освободился.\n", (unsigned long)pthread_self());
    }
    return NULL;
}


int main(){

    int server_fd, client_fd;
    struct Worker pool[5];

  for(int i = 0; i < POOL_SIZE; i++) {
        pool[i].is_free = 1;
        pool[i].client_fd = -1;
        pthread_mutex_init(&pool[i].mutex, NULL);
        pthread_cond_init(&pool[i].cond, NULL);
        
        if (pthread_create(&pool[i].tid, NULL, pthread_handle, (void*)&pool[i]) != 0) {
            perror("pthread_create error");
            exit(EXIT_FAILURE);
        }
    }

    struct sockaddr_in server_addr, client_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    socklen_t client_addr_len = sizeof(client_addr);
    socklen_t server_addr_len = sizeof(server_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    int flag = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    if(bind(server_fd, (struct sockaddr*) &server_addr, server_addr_len) == -1){
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 10) == -1){
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен...\n");

    while(1){

        if((client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &client_addr_len)) == -1){
            perror("accept error");
            continue;
        }

        int assigned = 0;

        for (int i = 0; i < POOL_SIZE; i++) { 
            if (pool[i].is_free == 0) {
                continue; 
            }
            
            pthread_mutex_lock(&pool[i].mutex);
            pool[i].client_fd = client_fd;
            pool[i].is_free = 0;
            pthread_mutex_unlock(&pool[i].mutex);
            pthread_cond_signal(&pool[i].cond);
            
            assigned = 1;
            printf("[Сервер] Передал клиента потоку с индексом %d.\n", i);
            break; 
        }
        
        if (!assigned) {
            printf("[Сервер] Все потоки заняты.\n");
            close(client_fd);
        }
        

    }


    close(server_fd);
    return 0;   


}