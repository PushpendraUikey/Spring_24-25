#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void handle_sigchld(int sig) {
    (void)sig; // Suppress unused parameter warning
    pid_t pid;
    int status;

    // Reap the child process   [-1 is for any child terminated reap that.]
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) { // if no child died written immediately[don't wait]
        if (WIFEXITED(status)) {    // True if normal exit() 
            printf("Child process (PID: %d) exited with status %d.\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {   // true if killed by signal
            printf("Child process (PID: %d) was terminated by signal %d.\n", pid, WTERMSIG(status));
        }
    }
}

int main() {
    pid_t pid;
    struct sigaction sa;

    // Set up signal handler for SIGCHLD
    sa.sa_handler = handle_sigchld;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);

    // Fork a new process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) is running...\n", getpid());
        sleep(30); // Child sleeps for a long time
        printf("Child process is exiting.\n");
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        printf("Parent process (PID: %d) is waiting for %d seconds before terminating the child.\n", getpid(), 10);
        sleep(10); // Parent sleeps for 10 seconds

        printf("Parent is terminating the child process (PID: %d).\n", pid);
        if (kill(pid, SIGTERM) == 0) {
            printf("Child process termination signal sent successfully.\n");
        } else {
            perror("Failed to terminate child process");
        }

        // Parent waits to reap the child
        sleep(1); // Brief sleep to allow signal handler to reap the child
        printf("Parent process is exiting.\n");
        exit(EXIT_SUCCESS);
    }
}
