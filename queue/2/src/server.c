
#include "server.h"


#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

static struct client clients[MAX_USERS];


static volatile sig_atomic_t server_should_exit = 0;

static void handle_signal(int signal)
{
    (void)signal;
    server_should_exit = 1;
}

static int add_client(struct client *clients, int *clients_count, long client_id, const char *name)
{
    for (int i = 0; i < *clients_count; ++i) {
        if (clients[i].client_id == client_id) {
            return 0;
        }
    }

    if (*clients_count == MAX_USERS) {
        return -1;
    }

    clients[*clients_count].client_id = client_id;
    strncpy(clients[*clients_count].name, name, sizeof(clients[*clients_count].name) - 1);
    clients[*clients_count].name[sizeof(clients[*clients_count].name) - 1] = '\0';
    (*clients_count)++;
    return 0;
}

int event_list_msg(struct client *clients, int clients_count, struct msg_buffer msg, int msgid){
                msg.mtype = msg.sender_id;
                msg.msg_type_ = EVENT_USER_JOINED;
                for (int i = 0; i < clients_count; ++i) {
                    strncpy(msg.username, clients[i].name, sizeof(msg.username) - 1);
                    msg.username[sizeof(msg.username) - 1] = '\0';
                    if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(msg.mtype), 0) == -1) {
                        perror("msgsnd");
                    }
                }  
                return 0; 
}



static int delete_client(struct client *clients, int *clients_count, long client_id)
{
    for (int i = 0; i < *clients_count; ++i) {
        if (clients[i].client_id == client_id) {
            clients[i] = clients[*clients_count - 1];
            (*clients_count)--;
            return 0;
        }
    }

    return -1;
}


int main(void){


    int clients_count = 0;
 
    int fd = open("/tmp/queue_keyfile", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    close(fd);
 
    key_t key = ftok("/tmp/queue_keyfile", 1);

    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget server");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    while(!server_should_exit){
        struct msg_buffer msg;
        if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(msg.mtype), CLIENT_TO_SERVER, 0) == -1) {
            if (errno == EINTR && server_should_exit) {
                break;
            }
            perror("msgrcv");
            break;
        }


        printf("Получено сообщение от %s: %s, ID: %ld, Тип: %d\n", msg.username, msg.mtext , msg.sender_id, msg.msg_type_);

        switch(msg.msg_type_) {
            case EVENT_CHAT_MESSAGE:
                for (int i = 0; i < clients_count; ++i) {
                msg.mtype = clients[i].client_id;
                   if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(msg.mtype), 0) == -1) {
                    perror("msgsnd");
                    break;
                    }
                }       
            break;
            case EVENT_USER_JOINED:
                if (add_client(clients, &clients_count, msg.sender_id, msg.username) == -1) {
                    fprintf(stderr, "Слишком много клиентов\n");
                    continue;
                }

                printf("Пользователь %s присоединился\n", msg.username);
                
                for (int i = 0; i < clients_count; ++i) {
                    msg.mtype = clients[i].client_id;
                    if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(msg.mtype), 0) == -1) {
                        perror("msgsnd");
                        break;
                    }
                }
                event_list_msg(clients, clients_count, msg, msgid);

            break;
            case EVENT_USER_LEFT:  printf("Пользователь %s покинул чат\n", msg.username);
                if (delete_client(clients, &clients_count, msg.sender_id) == -1) {
                    fprintf(stderr, "Ошибка при удалении клиента\n");
                }
                for (int i = 0; i < clients_count; ++i) {
                    msg.mtype = clients[i].client_id;
                    if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(msg.mtype), 0) == -1) {
                        perror("msgsnd");
                        break;
                    }
          
                }
            break;
            default: break;
        }

    }
    
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl IPC_RMID");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
