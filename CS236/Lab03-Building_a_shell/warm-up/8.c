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

    // suppresses cntrl+C of both parent and child
    signal(SIGINT, handle_sigint);      // cntrl+c signal handler it is!
    pid_t pid = fork();

    if(pid < 0){
        perror("fork failed!\n");
        exit(1);
    }else if(pid == 0){
        signal(SIGINT, handle_sigint);
        while(1){
            printf("I'm still running. There's nothing you can do now.\n");
            sleep(1);
        }
    }else{
        int i=0;
        while(i<5){
            printf("Keep trying to kill the child.\n");
            i+=1;
            sleep(2);
        }
        printf("Killing the child with pid %d\n", pid);
        kill(pid, SIGKILL);     // This one causes the kill of child forcefully[can't be caught or overwritten]
        wait(NULL);             // reap the child

        printf("Done with the parent as well.\n");
    }
}