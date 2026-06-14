#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#include "cli.h"
#include "driver.h"

#define MAX_LINE 256

volatile sig_atomic_t running = 1;

static void signal_handler(int signo)
{
    (void)signo;
    running = 0;
}

static int setup_signals(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction SIGINT");
        return -1;
    }

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction SIGTERM");
        return -1;
    }

    return 0;
}

int main(void)
{
    if (setup_signals() == -1) {
        return EXIT_FAILURE;
    }

    printf("Taxi CLI started\n");
    printf("Commands:\n");
    printf("  create_driver\n");
    printf("  send_task <pid> <task_timer>\n");
    printf("  get_status <pid>\n");
    printf("  get_drivers\n");
    printf("  exit\n");

    while (running) {
        struct pollfd fds[MAX_DRIVERS + 1];

        fds[0].fd = STDIN_FILENO;
        fds[0].events = POLLIN;
        fds[0].revents = 0;

        int nfds = 1;

        for (int i = 0; i < drivers_count; i++) {
            if (!drivers[i].used) {
                continue;
            }

            fds[nfds].fd = drivers[i].from_driver_fd;
            fds[nfds].events = POLLIN;
            fds[nfds].revents = 0;

            nfds++;
        }

        printf("> ");
        fflush(stdout);

        int ret = poll(fds, nfds, -1);

        if (ret == -1) {
            if (errno == EINTR) {
                continue;
            }

            perror("poll");
            break;
        }

        if (fds[0].revents & POLLIN) {
            char line[MAX_LINE];

            if (fgets(line, sizeof(line), stdin) == NULL) {
                running = 0;
                break;
            }

            handle_cli_cmd(line);
        }

        for (int i = 1; i < nfds; i++) {
            if (fds[i].revents & POLLIN) {
                handle_driver_response(fds[i].fd);
            }
        }
    }

    cleanup_drivers();

    printf("\nTaxi CLI stopped\n");

    return EXIT_SUCCESS;
}