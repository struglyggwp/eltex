#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(){

    pid_t fork_result = fork(); // Первый процесс
    pid_t fork_result2, fork_result3, fork_result4, fork_result5;

    if (fork_result < 0){
        perror("fork1 failed");
        exit(1);
    }


    if (fork_result == 0) {

        sleep(2);
        printf("Hello im 1 process!\n");
        printf("My PID: %d\n", getpid());
        printf("Parent PID: %d\n\n", getppid());
    
        fork_result3 = fork(); 
        if (fork_result3 < 0){

            perror("fork3 failed");
            exit(1);

        }
        if(fork_result3 == 0){ // Третий процесс

            sleep(2);
            printf("Hello im 3 process!\n");
            printf("My PID: %d\n", getpid());
            printf("My parent PID:%d\n\n", getppid());
            exit(0);
        }
        if(fork_result3 > 0){

            fork_result4 = fork();

            if (fork_result4 < 0){
                perror("fork4 failed");
                exit(1);
            }

            if(fork_result4 == 0){ // Четвертый процесс

                sleep(2);
                printf("Hello im 4 process!\n");
                printf("My PID: %d\n", getpid());
                printf("My parent PID:%d\n\n", getppid());
                exit(0);
            }
            if(fork_result4 > 0){   // Родительский процесс, который ждет завершения 3 и 4 процессов
                waitpid(fork_result3, NULL, 0);
                printf("3 process is finished\n");
                waitpid(fork_result4, NULL, 0);
                printf("4 process is finished\n");
                exit(0);
            }

        }
        

    }



    if (fork_result > 0) {
        sleep(2);
        printf("Hello im parent process!\n");
        printf("My PID: %d\n", getpid());
        printf("Parent PID: %d\n\n", getppid());


        fork_result2 = fork(); 

        if (fork_result2 < 0){
            perror("fork2 failed");
            exit(1);
        }

        if(fork_result2 == 0){ // Второй процесс

          //  sleep(2);
            printf("Hello im 2 process!\n");
            printf("My PID: %d\n", getpid());
            printf("My parent PID:%d\n\n", getppid());

            fork_result5 = fork();

            if (fork_result5 < 0){
                perror("fork5 failed");
                exit(1);
            }

            if(fork_result5 == 0){

             //   sleep(2);
                printf("Hello im 5 process!\n");
                printf("My PID: %d\n", getpid());
                printf("My parent PID:%d\n\n", getppid());
                exit(0);
            }
            if(fork_result5 > 0){ // второй процесс, который ждет завершения 5 процесса

                waitpid(fork_result5, NULL, 0);
                printf("5 process is finished\n");
                exit(0);
            }
        }
        if (fork_result2 > 0){ // Родительский процесс, который ждет завершения 2 процесса

            waitpid(fork_result, NULL, 0);
            printf("1 process is finished\n");
            waitpid(fork_result2, NULL, 0);
            printf("2 process is finished\n");

            printf("Parent process is finished\n");
            exit(0);
      
        }
    }
  
    return 0;

}
