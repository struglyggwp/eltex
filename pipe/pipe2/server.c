#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>



int main(){

const char* fifo_path = "/tmp/fifo";
    
    char buf[100];
    if(mkfifo(fifo_path, 0666) == -1){
        if(errno != EEXIST){
            perror("failed mkfifo");
            exit(EXIT_FAILURE);
        }
    }
    int fd_fifo;
    if ((fd_fifo = open(fifo_path, O_WRONLY)) == -1){
        perror("failed open fifo");
        exit(EXIT_FAILURE);
    }
    char message[50] = "Hello!";
    write(fd_fifo, message, strlen(message));

    close(fd_fifo);


}