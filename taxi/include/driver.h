#ifndef DRIVER_H
#define DRIVER_H

#include <sys/types.h>
#include <signal.h>

#define MAX_DRIVERS 64

enum cmd_type {
    CMD_TASK,
    CMD_STATUS,
    CMD_EXIT
};

enum resp_type {
    RESP_AVAILABLE,
    RESP_BUSY,
    RESP_ERROR
};

struct command {
    enum cmd_type type;
    int task_timer;
};

struct response {
    enum resp_type type;
    pid_t pid;
    int remaining_time;
};

struct driver {
    pid_t pid;

    int to_driver_fd;
    int from_driver_fd;

    int used;
};

extern struct driver drivers[MAX_DRIVERS];
extern int drivers_count;
extern volatile sig_atomic_t running;

void create_driver(void);
void send_task(pid_t pid, int timer);
void get_status(pid_t pid);
void get_drivers(void);
void cleanup_drivers(void);

void handle_driver_response(int fd);

#endif