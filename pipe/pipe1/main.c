#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void){
    int pipefd[2];
    pid_t cpid;
    char buf;  
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {  
        close(pipefd[1]);        
        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        _exit(EXIT_SUCCESS);

    }

        close(pipefd[0]);       
        char *message = "Hi!";
        write(pipefd[1], message, strlen(message));
        close(pipefd[1]);         
        wait(NULL);               
        exit(EXIT_SUCCESS);
    

}