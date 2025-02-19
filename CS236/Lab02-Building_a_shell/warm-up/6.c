#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// inconsistency in the print of last child is due to the scheduling by OS, but how?

int main(int argc, char* argv[]){
    int n=6;
    for(int i=0; i<n; i++){
        pid_t pid = fork();

        if(pid < 0){
            perror("Fork failed!\n");
            exit(EXIT_FAILURE);
        }else if(pid == 0){
            printf("Child %d with pid:%d and parent pid:%d\n", i+1, (int)getpid(), (int)getppid());
            exit(EXIT_SUCCESS);    // This exit causes child to return from here and don't move further.
        }
    }

    for(int i=0; i<n; i++){
        int status;
        pid_t child_pid = wait(&status);

        if(child_pid > 0){
            if(WIFEXITED(status)){
                printf("Child process %d exited with status %d\n", child_pid, WEXITSTATUS(status));
            }
        }else{
            perror("Wait failed\n");
        }
    }
}