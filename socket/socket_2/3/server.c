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
#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int queue_count = 0; 
int head = 0;       
int tail = 0;

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;


void* pthread_handle(void* arg) {

    (void)arg;

    while(1) {
        pthread_mutex_lock(&queue_mutex);

        while(queue_count == 0){
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }

        int fd = queue[head];
        head = (head + 1) % QUEUE_SIZE;
        queue_count--;
        pthread_mutex_unlock(&queue_mutex);

        printf("[Поток %lu] Взял клиента.\n", (unsigned long)pthread_self());

        time_t raw_time = time(NULL);
        char *time_str = ctime(&raw_time);
        
        sleep(2);

        send(fd, time_str, strlen(time_str), 0);
        close(fd);

        printf("[Поток %lu] Освободился.\n", (unsigned long)pthread_self());
    }
    return NULL;
}




int main(){


    int server_fd, client_fd;
    pthread_t pool[POOL_SIZE];

    for(int i = 0; i < POOL_SIZE; i++) {
        if (pthread_create(&pool[i], NULL, pthread_handle, NULL) != 0) {
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
       
        pthread_mutex_lock(&queue_mutex);

        if(queue_count != QUEUE_SIZE){
            queue[tail] = client_fd;
            tail = (tail + 1) % QUEUE_SIZE;
            queue_count++;
            pthread_cond_signal(&queue_cond);
        }else{
            printf("Очередь заполнена\n");
            close(client_fd);
        }

        pthread_mutex_unlock(&queue_mutex);
            
        

    }


    close(server_fd);
    return 0;   


}



