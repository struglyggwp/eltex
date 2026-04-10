#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>



int main(){
    while(1){
        char command[100]; 
        printf("eltexshell> "); 
        fflush(stdout);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;  
        
        char *args[10]; 
        int i = 0;

        args[i] = strtok(command, " "); 
        if (args[i] == NULL) {
            continue;
        }
        i++;

        while (i < 9 && (args[i] = strtok(NULL, " ")) != NULL) {  
            i++;
        }
        args[i] = NULL;

        if (strcmp(args[0], "exit") == 0) {
           break;
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
            exit(EXIT_FAILURE);
        }
    }
}
