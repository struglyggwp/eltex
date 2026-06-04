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

    int fd_fifo;
    if ((fd_fifo = open(fifo_path, O_RDONLY)) == -1){
        perror("failed open fifo");
        exit(EXIT_FAILURE);
    }
    read(fd_fifo, buf, sizeof(buf));
    printf("Received message: %s\n", buf);
    
    close(fd_fifo);

    if(unlink(fifo_path) == -1){
        perror("failed unlink fifo");
        exit(EXIT_FAILURE);
    }
    return 0;

}
