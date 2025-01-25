#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void handle_sigint(int sig){
    (void)sig; // Suppress unused parameter warning

    printf("I'll run forever.\n");

}
int main(int argc, char* argv[]){

    signal(SIGINT, handle_sigint);
    pid_t pid = fork();

    if(pid < 0){
        perror("fork failed!\n");
        exit(1);
    }else if(pid == 0){
        signal(SIGINT, handle_sigint);
        while(1){
            printf("I'm still running. There's nothing you can do now.\n");
            sleep(1000);
        }
    }else{
        int i=0;
        while(i<15){
            printf("Keep trying to kill the child.\n");
            i+=1;
            sleep(500000);
        }
        printf("Killing the child with pid %d\n", pid);
        kill(pid, SIGKILL);
        wait(NULL);     // reap the parent

        printf("Done with the parent as well.\n");
    }
}