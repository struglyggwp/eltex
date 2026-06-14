#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#include "driver.h"

struct driver drivers[MAX_DRIVERS];
int drivers_count = 0;

static int find_driver_index(pid_t pid)
{
    for (int i = 0; i < drivers_count; i++) {
        if (drivers[i].used && drivers[i].pid == pid) {
            return i;
        }
    }

    return -1;
}

static void print_response(const struct response *resp)
{
    if (resp->type == RESP_AVAILABLE) {
        printf("Driver %d: Available\n", resp->pid);
    } else if (resp->type == RESP_BUSY) {
        printf("Driver %d: Busy %d\n", resp->pid, resp->remaining_time);
    } else {
        printf("Driver %d: Error\n", resp->pid);
    }
}

static void send_response(int write_fd, const struct response *resp)
{
    ssize_t ret = write(write_fd, resp, sizeof(*resp));

    if (ret == -1) {
        perror("write response");
        return;
    }

    if (ret != sizeof(*resp)) {
        fprintf(stderr, "driver: incomplete response write\n");
    }
}

static void driver_loop(int read_fd, int write_fd)
{
    int busy = 0;
    time_t busy_until = 0;

    while (1) {
        struct command cmd;

        ssize_t ret = read(read_fd, &cmd, sizeof(cmd));

        if (ret == 0) {
            break;
        }

        if (ret == -1) {
            perror("read command");
            break;
        }

        if (ret != sizeof(cmd)) {
            fprintf(stderr, "driver: incomplete command read\n");
            continue;
        }

        time_t now = time(NULL);

        if (busy && now >= busy_until) {
            busy = 0;
            busy_until = 0;
        }

        struct response resp;
        memset(&resp, 0, sizeof(resp));

        resp.pid = getpid();

        if (cmd.type == CMD_EXIT) {
            break;
        }

        if (cmd.type == CMD_STATUS) {
            if (busy) {
                resp.type = RESP_BUSY;
                resp.remaining_time = (int)(busy_until - now);
            } else {
                resp.type = RESP_AVAILABLE;
                resp.remaining_time = 0;
            }

            send_response(write_fd, &resp);
        } else if (cmd.type == CMD_TASK) {
            if (busy) {
                resp.type = RESP_BUSY;
                resp.remaining_time = (int)(busy_until - now);
            } else {
                busy = 1;
                busy_until = now + cmd.task_timer;

                resp.type = RESP_BUSY;
                resp.remaining_time = cmd.task_timer;
            }

            send_response(write_fd, &resp);
        } else {
            resp.type = RESP_ERROR;
            resp.remaining_time = 0;

            send_response(write_fd, &resp);
        }
    }

    close(read_fd);
    close(write_fd);

    _exit(EXIT_SUCCESS);
}

void create_driver(void)
{
    if (drivers_count >= MAX_DRIVERS) {
        printf("Driver limit reached\n");
        return;
    }

    int to_driver[2];
    int from_driver[2];

    if (pipe(to_driver) == -1) {
        perror("pipe to_driver");
        return;
    }

    if (pipe(from_driver) == -1) {
        perror("pipe from_driver");

        close(to_driver[0]);
        close(to_driver[1]);

        return;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");

        close(to_driver[0]);
        close(to_driver[1]);
        close(from_driver[0]);
        close(from_driver[1]);

        return;
    }

    if (pid == 0) {
        close(to_driver[1]);
        close(from_driver[0]);

        driver_loop(to_driver[0], from_driver[1]);
    }

    close(to_driver[0]);
    close(from_driver[1]);

    drivers[drivers_count].pid = pid;
    drivers[drivers_count].to_driver_fd = to_driver[1];
    drivers[drivers_count].from_driver_fd = from_driver[0];
    drivers[drivers_count].used = 1;

    drivers_count++;

    printf("Created driver with PID %d\n", pid);
}

void send_task(pid_t pid, int timer)
{
    int index = find_driver_index(pid);

    if (index == -1) {
        printf("Driver %d not found\n", pid);
        return;
    }

    struct command cmd;

    cmd.type = CMD_TASK;
    cmd.task_timer = timer;

    ssize_t ret = write(drivers[index].to_driver_fd, &cmd, sizeof(cmd));

    if (ret == -1) {
        perror("write task command");
        return;
    }

    if (ret != sizeof(cmd)) {
        fprintf(stderr, "cli: incomplete task command write\n");
    }
}

void get_status(pid_t pid)
{
    int index = find_driver_index(pid);

    if (index == -1) {
        printf("Driver %d not found\n", pid);
        return;
    }

    struct command cmd;

    cmd.type = CMD_STATUS;
    cmd.task_timer = 0;

    ssize_t ret = write(drivers[index].to_driver_fd, &cmd, sizeof(cmd));

    if (ret == -1) {
        perror("write status command");
        return;
    }

    if (ret != sizeof(cmd)) {
        fprintf(stderr, "cli: incomplete status command write\n");
    }
}

void get_drivers(void)
{
    if (drivers_count == 0) {
        printf("No drivers\n");
        return;
    }

    for (int i = 0; i < drivers_count; i++) {
        if (!drivers[i].used) {
            continue;
        }

        get_status(drivers[i].pid);
    }
}

void cleanup_drivers(void)
{
    struct command cmd;

    cmd.type = CMD_EXIT;
    cmd.task_timer = 0;

    for (int i = 0; i < drivers_count; i++) {
        if (!drivers[i].used) {
            continue;
        }

        ssize_t ret = write(drivers[i].to_driver_fd, &cmd, sizeof(cmd));

        if (ret == -1) {
            perror("write exit command");
        }

        close(drivers[i].to_driver_fd);
        close(drivers[i].from_driver_fd);

        waitpid(drivers[i].pid, NULL, 0);

        drivers[i].used = 0;
    }
}

void handle_driver_response(int fd)
{
    struct response resp;

    ssize_t ret = read(fd, &resp, sizeof(resp));

    if (ret == sizeof(resp)) {
        print_response(&resp);
    } else if (ret == 0) {
        printf("Driver pipe closed\n");
    } else if (ret == -1) {
        perror("read response");
    } else {
        fprintf(stderr, "cli: incomplete response read\n");
    }
}