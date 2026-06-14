#include <string.h>
#include <stdio.h>

#include "cli.h"
#include "driver.h"

void handle_cli_cmd(char *line)
{
    char command[64];
    pid_t pid;
    int timer;

    if (sscanf(line, "%63s", command) != 1) {
        return;
    }

    if (strcmp(command, "create_driver") == 0) {
        create_driver();
    } else if (strcmp(command, "send_task") == 0) {
        if (sscanf(line, "%*s %d %d", &pid, &timer) != 2) {
            printf("Usage: send_task <pid> <task_timer>\n");
            return;
        }

        if (timer <= 0) {
            printf("task_timer must be > 0\n");
            return;
        }

        send_task(pid, timer);
    } else if (strcmp(command, "get_status") == 0) {
        if (sscanf(line, "%*s %d", &pid) != 1) {
            printf("Usage: get_status <pid>\n");
            return;
        }

        get_status(pid);
    } else if (strcmp(command, "get_drivers") == 0) {
        get_drivers();
    } else if (strcmp(command, "exit") == 0) {
        running = 0;
    } else {
        printf("Unknown command\n");
    }
}