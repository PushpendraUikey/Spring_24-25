#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

int my_chdir(const char *path) {
    // Validate the input path
    if (path == NULL) {
        errno = EINVAL; // Invalid argument
        return -1;
    }

    // Check if the path exists and is a directory
    struct stat statbuf;
    if (stat(path, &statbuf) < 0) {
        // Path doesn't exist or can't be accessed
        return -1;
    }

    if (!S_ISDIR(statbuf.st_mode)) {
        // Path exists but is not a directory
        errno = ENOTDIR; // Not a directory
        return -1;
    }

    // Use the system chdir to actually change the working directory
    if (chdir(path) < 0) {
        // Failed to change the directory
        return -1;
    }

    return 0; // Success
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    const char *new_dir = argv[1];

    if (my_chdir(new_dir) == 0) {
        printf("Successfully changed directory to: %s\n", new_dir);

        // Display current working directory
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Current working directory: %s\n", cwd);
        } else {
            perror("getcwd");
        }
    } else {
        fprintf(stderr, "Failed to change directory to: %s\n", new_dir);
        perror("Error");
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens */
char **tokenize(char *line) {
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0;

    for (i = 0; i < strlen(line); i++) {
        char readChar = line[i];

        if (readChar == ' ' || readChar == '\n' || readChar == '\t') {
            token[tokenIndex] = '\0';
            if (tokenIndex != 0) {
                tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0;
            }
        } else {
            token[tokenIndex++] = readChar;
        }
    }

    free(token);
    tokens[tokenNo] = NULL;
    return tokens;
}

int main() {
    char line[MAX_INPUT_SIZE];
    char **tokens;

    while (1) {
        printf("$ ");
        bzero(line, sizeof(line));
        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = 0; // Remove trailing newline

        tokens = tokenize(line);

        if (tokens[0] == NULL) {
            // Empty input
            continue;
        }

        // Handle exit command
        if (strcmp(tokens[0], "exit") == 0) {
            printf("Exiting shell.\n");
            break;
        }

        // Check for background execution
        int runInBackground = 0;
        int lastIndex = 0;

        // Find last token index
        for (lastIndex = 0; tokens[lastIndex] != NULL; lastIndex++);

        lastIndex--; // Move to the last token
        if (strcmp(tokens[lastIndex], "&") == 0) {
            runInBackground = 1;
            free(tokens[lastIndex]);
            tokens[lastIndex] = NULL; // Remove '&' from the tokens
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
        } else if (pid == 0) {
            // Child process
            if (execvp(tokens[0], tokens) < 0) {
                perror("Command execution failed");
                exit(1);
            }
        } else {
            // Parent process
            if (!runInBackground) {
                // Wait for the child to complete if not running in background
                waitpid(pid, NULL, 0);
            } else {
                printf("Process running in background with PID: %d\n", pid);
            }
        }

        // Free allocated memory
        for (int i = 0; tokens[i] != NULL; i++) {
            free(tokens[i]);
        }
        free(tokens);
    }

    return 0;
}
