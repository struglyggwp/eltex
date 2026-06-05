#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>



int main(void)
{
    printf("Мой PID - %d\n", getpid());

    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGUSR1);

    sigprocmask(SIG_BLOCK, &set, NULL);

    int sig = 0;

    while(1){
        sigwait(&set, &sig);

        if(sig == SIGUSR1){
            printf("Пришел сигнал SIGUSR1\n");
        }
    }

    return EXIT_SUCCESS;
}
