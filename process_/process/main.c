#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    int fork_result = fork();

    if(fork_result == -1){
        printf("Fork failed\n");
        return 1;
    }
    if (fork_result == 0){
        printf("My PID is %d, am child\n", getpid());
        printf("My PPID is %d\n\n", getppid());
        exit(0);
    }
    if(fork_result > 0){
        printf("My PID is %d, am parent\n", getpid());
        printf("My  PPID is %d\n", getppid());
        wait(NULL);
    }

    return 0;
}
