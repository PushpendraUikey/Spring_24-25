#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){

    if(argc == 1){
        printf("Usage: %s <commands>\n", argv[0]);
        return 1;
    }
    pid_t pid = fork();

    if(pid<0){
        perror("Fork failed!\n");
        exit(EXIT_FAILURE);
    }else if(pid == 0){
        printf("This line is before <exec> command.\n");
        execvp(argv[1], &argv[1]);
        printf("This line is after <exec> command.\n");
    }else{
        int ret = wait(NULL);
        if(ret>0){
            printf("Child with pid:%d executed successfully. (pid:%d)\n", ret, (int)getpid());
        }else{
            printf("Child of current process (pid:%d) didn't terminated successfully.\n", (int)getpid());
        }
    }
}