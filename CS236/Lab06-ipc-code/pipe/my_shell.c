#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>

// In this code I'll be implementing the pipe functionality in the shell code
// Assuming that there won't be parallel background execution if pipe used
// I'll be using the multiexec array to execute the pipe


#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_MULTI_COMMANDS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

void handle_sigint(int sig){
    (void)sig; // Suppress unused parameter warning
	// do nothing just catch the ctrl+c
}

void free_mem(char ** tokens){
	for(i=0;tokens[i]!=NULL;i++){
		free(tokens[i]);
	}
	free(tokens);
}

int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;
	char **XX__multiexec[MAX_MULTI_COMMANDS];
	for (int i = 0; i < MAX_MULTI_COMMANDS; i++) {
		XX__multiexec[i] = (char **)malloc(MAX_MULTI_COMMANDS * sizeof(char *));
	}

	int XX__multisize = 0;
	bool XX__ismulti = false;
    bool isPipe = false;
	int wc, rc;
	int i, j;

	int period = 0;
	signal(SIGINT, handle_sigint);	// This also preventing the our shell to not exit
	while(1) {			
		/* BEGIN: TAKING INPUT */

		// Old inputing method!
		bzero(line, sizeof(line));
		char *cwd = getcwd(NULL, 0); 	// Dynamically allocate cwd
		if(cwd != NULL){
			printf("%s$ ", cwd);
			free(cwd);
		}else{
			printf("$ ");
		}
		scanf("%[^\n]", line);
		getchar();

		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);		


		// empty command
		if(tokens[0] == NULL){
			period++;
			continue;
		}

		if(period%3==0){			// reaping the zombie processess periodically!
			int wc;
		// WNOHANG: it actually allows waitpid to not wait for any child to become zombie but it 
		// immediately returns zero if no process is in zombie state. [ i.e. it won't wait for the processes which are running in the background]
			if((wc = waitpid(-1, NULL, WNOHANG)) > 0) {	
				printf("Shell: Background process reaped with pid %d\n", wc);
			}
		}
		period++;

  		if (strcmp(tokens[0], "exit") == 0) {
            while ((wc = waitpid(-1, NULL, 0)) > 0) {	// reaping all the background process
                // It'll wait for each child to finish until none left.
                    printf("Shell: Background process finished with pid %d\n", wc);
            }
            free_mem(tokens);
            printf("Exiting shell.\n");
            break;
        }

		// background process check
        int runInBackground = 0;
        int lastIndex = 0;
		int k=0;
        for (lastIndex = 0; tokens[lastIndex] != NULL; lastIndex++){
            if(strcmp(tokens[lastIndex], "&&")==0){
                XX__multiexec[XX__multisize++][k] = NULL;
                XX__ismulti = true;
                k = 0;
            }else if(strcmp(tokens[lastIndex],"|")==0){  // Pipe check
                isPipe = true;
                XX__multiexec[XX__multisize++][k] = NULL;
                k = 0;
            }else{
                XX__multiexec[XX__multisize][k] = strdup(tokens[lastIndex]);
                k++;
            }
        }
		XX__multiexec[XX__multisize++][k] = NULL;		// Last one make NULL terminated

		if(XX__ismulti){
			for(int j=0; j<XX__multisize; j++){
				// cd command processing
				if (XX__multiexec[j][0] != NULL && strcmp(XX__multiexec[j][0], "cd") == 0) {
					if (XX__multiexec[j][1]== NULL) {
						printf("Error: No directory specified.\n");
					} else if (chdir(XX__multiexec[j][1]) < 0) {
						perror("Error: No such directory exists.\n"); 
					}
					continue;
				}

				int rc = fork();
				if(rc==0){
					execvp(XX__multiexec[j][0], XX__multiexec[j]);
					printf("%s: Invalid command!\n", XX__multiexec[j][0]);
					exit(EXIT_FAILURE);
				}else if(rc>0){
					// each parent process has to wait for the child to be reaped.
					int wc = waitpid(rc, NULL, 0);
				}
			}
			for(i=0;tokens[i]!=NULL;i++){
				free(tokens[i]);
			}
			free(tokens);
			XX__multisize = 0;
			XX__ismulti = false;	// Reset for new command
		}
        
        else if(isPipe)
		
		{
			if(XX__multisize < 2){
				printf("Error: incomplete arguments to pipe\n");
				free_mem(tokens);
				isPipe = false;
				XX__multisize = 0;
				continue;
			}
			int pipefd[2], sfd[2];
            for(i=0; i<XX__multisize; i++){
                if(pipe(pipefd) == -1){
					perror("pipe failed\n");
					break;
				}
				if(pipe(sfd) == -1){
					perror("pipe failed\n");
					break;
				}
				wc = fork();
				if(wc == 0){
					close(pipefd[1]);
					dup2(pipefd[0], STDIN_FILENO);
					close(pipefd[0]);
					close(sfd[0]);
					dup2(sfd[1], STDOUT_FILENO);
					close(sfd[1]);

				}
            }

			free_mem(tokens);
			isPipe = false;
			XX__multisize = 0;
        }

		else
				// Non-multi commands goes here.
		{
			lastIndex--; 					// Move to the last token
			if (strcmp(tokens[lastIndex], "&") == 0) {
				runInBackground = 1;
				free(tokens[lastIndex]);
				tokens[lastIndex] = NULL; 	// Remove '&' from the tokens
			}

			// cd command processing
			if (tokens[0] != NULL && strcmp(tokens[0], "cd") == 0) {
				if (tokens[1] == NULL) {
					printf("Error: No directory specified.\n");
				} else if (chdir(tokens[1]) < 0) {
					printf("Error: <%s> No such directory exists.\n", tokens[1]); 
				}
				continue;
			}

			int rc = fork();
			if(rc==0){
				if(runInBackground) signal(SIGINT, handle_sigint);// if it is a background process then suppress cntrl+c
				execvp(tokens[0], tokens);
				printf("%s: Invalid command!\n", tokens[0]);
				exit(EXIT_FAILURE);
			}else if(rc>0){
				if(!runInBackground){		// if running in the background then don't wait!
					int wc = waitpid(rc, NULL, 0);
				}
			}
		
			free_mem(tokens);
			XX__multisize = 0;
			XX__ismulti = false;
		}
	}

	// In the end free up everything the memorie's taken and exit the shell.
	for (i = 0; i < MAX_MULTI_COMMANDS; i++) {
		if (XX__multiexec[i] != NULL) {
			for (int j = 0; j < MAX_MULTI_COMMANDS; j++) {
				if(XX__multiexec[i][j] != NULL) free(XX__multiexec[i][j]);	// free up everything allocated
			}
			free(XX__multiexec[i]);
		}
	}
	return 0;
}
