#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
    int pid = fork();

    if(pid<0){
        perror("fork failed");
        exit(EXIT_FAILURE);
    }else if(pid==0){
        printf("I'm the child process with (PID: %d)\n", (int)getpid());
        sleep(2);
        exit(1);
    }
    else {
        int status;
        int ret = waitpid(pid, &status, 0); // waits for specific child to terminate[waitpid is used to keep track of status]

        printf("I'm the parent process of child:%d with (PID: %d)\n", pid, (int)getpid());
        if(ret>0){
            if(WIFEXITED(status)){
                printf("child exited with status: %d\n", WEXITSTATUS(status));
            }else{
                printf("Child didn't terminated properly.\n");
            }
        }else{
            perror("waitpid failed.\n");
        }
    }
}