#define _POSIX_C_SOURCE 200809L


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

static void sigusr1_handler(int signo)
{
    (void)signo;

    const char message[] = "Получен сигнал SIGUSR1\n";
    write(STDOUT_FILENO, message, sizeof(message) - 1);
}

int main(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = sigusr1_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    printf("Программа-получатель запущена.\n");
    printf("PID процесса: %ld\n", (long)getpid());
    printf("Ожидание сигнала SIGUSR1...\n");

    fflush(stdout);

    while (1) {
        pause();
    }

    return EXIT_SUCCESS;
}