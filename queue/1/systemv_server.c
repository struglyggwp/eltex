#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

struct message{
    long m_type;
    char m_text[128];
};



int main(){

    const char path[] = "/tmp/key_pass";

    int fd = open(path, O_CREAT | O_RDWR, 0666);
    if (fd == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }
    close(fd);


    key_t key = ftok(path, 'A');
    if (key == -1){
        perror("key");
        exit(EXIT_FAILURE);
    }
    int msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid == -1){
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    
    struct message in = {.m_type = 1, .m_text = "Hi am server"};
    struct message out;

    if (msgsnd(msqid, &in, strlen(in.m_text) + 1, 0 ) == -1){
        perror("msgsnd");
        exit(EXIT_FAILURE); 
    }

    if(msgrcv(msqid, &out, sizeof(out.m_text), 2, 0) == -1){
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    
     if( msgctl(msqid, IPC_RMID, NULL) == -1){
        perror("msgctl");
        exit(EXIT_FAILURE);
    }


    printf("%s\n", out.m_text);

}
