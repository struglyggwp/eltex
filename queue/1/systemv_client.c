#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message{
    long m_type;
    char m_text[128];
};

int main(){

    const char path[] = "/tmp/key_pass";

    key_t key = ftok(path, 'A');
    if (key == -1){
        perror("key");
        exit(EXIT_FAILURE);
    }

    int msqid = msgget(key, 0);
    if (msqid == -1){
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    struct message in = { .m_type = 2, .m_text = "Hello am client"};
    struct message out;

    if(msgrcv(msqid, &out, sizeof(out.m_text), 1, 0) == -1){
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    if(msgsnd(msqid, &in, strlen(in.m_text) + 1, 0) == -1){
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", out.m_text);

   
}
