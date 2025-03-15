#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    int n=3;
    int noofWait = 0;
    for(int i=0; i<n; i++){
        pid_t pid = fork();
    
        if(pid < 0){
            perror("Fork failed!\n");
            exit(EXIT_FAILURE);
        }else if(pid == 0){
            noofWait = 0;       // Child shouldn't have to wait additionaly.
            printf("Child %d with pid:%d and parent pid:%d\n", i+1, (int)getpid(), (int)getppid());
            // exit(EXIT_SUCCESS);    // This exit causes child to return from here and don't move further.
        }else{
            noofWait++;         // if parent then count it's children
        }
    }

    for(int i=0; i<noofWait; i++){
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