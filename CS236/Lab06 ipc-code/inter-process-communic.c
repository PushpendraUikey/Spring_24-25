#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int pipefd[2];
    pid_t pid;
    char buffer [500];  // To take the string input

    if(pipe(pipefd) == -1){
        perror("pipe failed\n");
        exit(1);
    }
    pid = fork();   // fork child

    if(pid > 0){
        // parent process
        close(pipefd[0]);   // not gonna read from the parent 
        dup2(pipefd[1], STDOUT_FILENO);     // output of parent goes to the pipe(Redirectin stdout to write end)
        close(pipefd[1]);   // Now no  longer need of it.
        // If you don't close the write end after duplicating it, you'll leave an extra open file descriptor. 
        // This can prevent the reading process from seeing an end-of-file (EOF) because the pipe remains 
        // open for writing. In other words, the reader may block indefinitely waiting for 
        // more data, even when no actual output is being produced.
        printf("Hello Baap bol rha hun! Kaisa hai, paise to nhi lagenge, khana wagarah sab shi chalrha? Tabiyat wagarah sab thik, pariksha wagarah me kya haal, padhayi me man lg rha ki nhi\n");
        fflush(stdout);     // ensuring that data is flushed 
        wait(NULL);
    }else if(pid == 0){
        close(pipefd[1]);   // close write end of pipe(no longer need it)
        dup2(pipefd[0], STDIN_FILENO);  // redirecting standard input to the read end of file
        close(pipefd[0]);

        // Now child's stdin is from pipe so we can use standard input functions
        fgets(buffer, sizeof(buffer), stdin);   // actually taking input from pipe
        printf("Child received: %s", buffer);
    }
}
