#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage: %s <command1> <command2>\n", argv[0]);
        return 1;
    }

    pid_t pid = fork();

    if(pid<0){
        perror("fork failed");
        exit(EXIT_FAILURE);
    }else if(pid == 0){
        if(execvp(argv[1], &argv[1])==-1){  // argv[] array is NULL terminated by shell
            perror("exec failed\n");
            exit(-1);
        }
    }else{
        int status;
        int ret = waitpid(pid, &status, 0); // waits for its child with pid given
        if(ret>0){
            if(WIFEXITED(status)){
                printf("Command successfully completed.\n");
            }else{
                printf("command didn't executed properly.\n");
            }
        }else{
            perror("Invalid arguments.\n");
        }
    }
}