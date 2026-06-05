#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>



int main(void)
{
    printf("PID процесса: %d\n", getpid());

    sigset_t sa;
    sigemptyset(&sa);

    sigaddset(&sa, SIGINT);

    if(sigprocmask(SIG_BLOCK, &sa, NULL) == -1){
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    printf("SIGINT заблокирован\n");
    printf("PID процесса: %d\n", getpid());

    while (1) {
        pause();
    }

    return EXIT_SUCCESS;
}
