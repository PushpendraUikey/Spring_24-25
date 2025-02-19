#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>

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

int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;
	char **XX__multiexec[MAX_MULTI_COMMANDS];
	for (int i = 0; i < MAX_MULTI_COMMANDS; i++) {
		XX__multiexec[i] = (char **)malloc(MAX_MULTI_COMMANDS * sizeof(char *));
	}

	int XX__multisize = 0;
	bool XX__ismulti = false;
	int i;

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

		// This can be implemented if we use a Data Structure to hold the pid's of each child processes
		// We have to have maxchild number to store in the fixed sized array.

		// if (strcmp(tokens[0], "exit") == 0) {
		// 	pid_t pid = getpid();   // Get the shell's PID
		// 	printf("Exiting shell.\n");

		// 	for (i = 0; i < max_child_count; i++) { // Terminate and reap all child processes
		// 		if (child_pids[i] > 0 && child_pids[i] != pid) {
		// 			kill(child_pids[i], SIGTERM); // Terminate each child process explicitly
		// 		}
		// 	}
		// 	// Reap all terminated child processes
		// 	int wc;
		// 	while ((wc = waitpid(-1, NULL, WNOHANG)) > 0) { 
		// 		printf("Shell: Background process finished with pid %d\n", wc);
		// 	}

		// 	// Free dynamically allocated memory
		// 	for (i = 0; tokens[i] != NULL; i++) {
		// 		free(tokens[i]);
		// 	}
		// 	free(tokens);

		// 	break; // Exit the shell
		// }

  		if (strcmp(tokens[0], "exit") == 0) {
            int wc;
			// kill(0, SIGTERM);// Not good as terminates entire group of current process including self.
			// -1 in place of pid: any process waiting to be reaped
				while ((wc = waitpid(-1, NULL, 0)) > 0) {	// reaping all the foreground and background process
					// It'll wait for each child to finish until none left.
						printf("Shell: Background process finished with pid %d\n", wc);
				}
				for(i=0;tokens[i]!=NULL;i++){
					free(tokens[i]);
				}
				free(tokens);
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
					// if(runInBackground) signal(SIGINT, handle_sigint); //if it is a background process then suppress ctrl+c
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
			if(period%5==0){			// reaping the zombie processess periodically!
				int wc;
			// WNOHANG: it actually allows waitpid to not wait for any child to become zombie but it 
			// immediately returns zero if no process is in zombie state. [ i.e. it won't wait for the processes which are running in the background]
				if((wc = waitpid(-1, NULL, WNOHANG)) > 0) {	
					printf("Shell: Background process reaped with pid %d\n", wc);
				}
			}
			period++;
		
			// Freeing the allocated memory	
			for(i=0;tokens[i]!=NULL;i++){
				free(tokens[i]);
			}
			free(tokens);

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
