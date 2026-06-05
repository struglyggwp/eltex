#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <PID>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr = NULL;
    errno = 0;

    long pid_value = strtol(argv[1], &endptr, 10);

    if (errno != 0 || *endptr != '\0' || pid_value <= 0 || pid_value > INT_MAX) {
        fprintf(stderr, "Ошибка: некорректный PID\n");
        return EXIT_FAILURE;
    }

    pid_t pid = (pid_t)pid_value;

    if (kill(pid, SIGUSR1) == -1) {
        perror("kill");
        return EXIT_FAILURE;
    }

    printf("Сигнал SIGUSR1 отправлен процессу с PID %ld\n", pid_value);

    return EXIT_SUCCESS;
}