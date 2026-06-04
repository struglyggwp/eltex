#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_CMD 100
#define MAX_ARGS 10

void parse_args(char *command, char *args[])
{
    int i = 0;

    args[i] = strtok(command, " ");

    if (args[i] == NULL) {
        return;
    }

    i++;

    while (i < MAX_ARGS - 1 && (args[i] = strtok(NULL, " ")) != NULL) {
        i++;
    }

    args[i] = NULL;
}

void execute_simple_command(char *command)
{
    char *args[MAX_ARGS];

    parse_args(command, args);

    if (args[0] == NULL) {
        return;
    }

    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    pid_t pid = fork();

    if (pid == 0) {
        execvp(args[0], args);
        perror("execvp failed");
        _exit(EXIT_FAILURE);
    } else if (pid > 0) {
        waitpid(pid, NULL, 0);
    } else {
        perror("fork failed");
    }
}

void execute_pipe_command(char *left_command, char *right_command)
{
    int pipefd[2];

    char *left_args[MAX_ARGS];
    char *right_args[MAX_ARGS];

    parse_args(left_command, left_args);
    parse_args(right_command, right_args);

    if (left_args[0] == NULL || right_args[0] == NULL) {
        fprintf(stderr, "Ошибка: пустая команда\n");
        return;
    }

    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return;
    }

    pid_t pid1 = fork();

    if (pid1 == -1) {
        perror("fork failed");
        close(pipefd[0]);
        close(pipefd[1]);
        return;
    }

    if (pid1 == 0) {
     

        close(pipefd[0]);

        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2 failed");
            _exit(EXIT_FAILURE);
        }

        close(pipefd[1]);

        execvp(left_args[0], left_args);

        perror("execvp left command failed");
        _exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();

    if (pid2 == -1) {
        perror("fork failed");
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid1, NULL, 0);
        return;
    }

    if (pid2 == 0) {
       

        close(pipefd[1]);

        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2 failed");
            _exit(EXIT_FAILURE);
        }

        close(pipefd[0]);

        execvp(right_args[0], right_args);

        perror("execvp right command failed");
        _exit(EXIT_FAILURE);
    }

 

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

int main(void)
{
    while (1) {
        char command[MAX_CMD];

        printf("eltexshell> ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (command[0] == '\0') {
            continue;
        }

        if (strcmp(command, "exit") == 0) {
            break;
        }

       

        char *pipe_pos = strchr(command, '|');

        if (pipe_pos == NULL) {
            execute_simple_command(command);
        } else {
        

            *pipe_pos = '\0';

            char *left_command = command;
            char *right_command = pipe_pos + 1;

            execute_pipe_command(left_command, right_command);
        }
    }

    return 0;
}