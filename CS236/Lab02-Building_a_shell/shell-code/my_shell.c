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
	char **__multiexec[MAX_MULTI_COMMANDS];
	for (int i = 0; i < MAX_MULTI_COMMANDS; i++) {
		__multiexec[i] = (char **)malloc(MAX_MULTI_COMMANDS * sizeof(char *));
	}

	int __multisize = 0;
	bool ismulti = false;
	int i;

	int period = 0;
	signal(SIGINT, handle_sigint);
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

  		if (strcmp(tokens[0], "exit") == 0) {
            int wc;
						kill(0, SIGTERM);	// kill the processes which are running in the background, foreground as child of the curr group.
			// -1 in place of pid: any process waiting to be reaped(i.e. it is in zombie state gets reaped)
					while ((wc = waitpid(-1, NULL, 0)) > 0) {	// reaping all the foreground and background process
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
						__multiexec[__multisize++][k] = NULL;
						ismulti = true;
						k = 0;
					}else{
						__multiexec[__multisize][k] = strdup(tokens[lastIndex]);
						k++;
					}
				}
				__multiexec[__multisize++][k] = NULL;		// Last one make NULL terminated

		if(ismulti){
			for(int j=0; j<__multisize; j++){
				// cd command processing
				if (__multiexec[j][0] != NULL && strcmp(__multiexec[j][0], "cd") == 0) {
					if (__multiexec[j][1]== NULL) {
						printf("Error: No directory specified.\n");
					} else if (chdir(__multiexec[j][1]) < 0) {
						perror("Error: No such directory exists.\n"); 
					}
					continue;
				}

				int rc = fork();
				if(rc==0){
					// if(runInBackground) signal(SIGINT, handle_sigint); //if it is a background process then suppress ctrl+c
					execvp(__multiexec[j][0], __multiexec[j]);
					perror("Invalid command!\n");
					exit(EXIT_FAILURE);
				}else if(rc>0){
					int wc = waitpid(rc, NULL, 0);
				}
			}
			for(i=0;tokens[i]!=NULL;i++){
				free(tokens[i]);
			}
			free(tokens);
			__multisize = 0;
			ismulti = false;	// Reset for new command
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
					perror("Error: No such directory exists.\n"); 
				}
				continue;
			}

			int rc = fork();
			if(rc==0){
				if(runInBackground) signal(SIGINT, handle_sigint);// if it is a background process then suppress cntrl+c
				execvp(tokens[0], tokens);
				perror("Invalid command!\n");
				exit(EXIT_FAILURE);
			}else if(rc>0){
				if(!runInBackground){		// if running in the background then don't wait!
					int wc = waitpid(rc, NULL, 0);
				}
			}
			if(period%2==0){			// reaping the zombie processess periodically!
				int wc;
			// WNOHANG: it actually allows waitpid to not wait for any child to become zombie but it 
			// immediately returns zero if no process is in zombie state. [ i.e. it won't wait for the processes which are running in the background]
				if((wc = waitpid(-1, NULL, WNOHANG)) > 0) {	
					printf("Shell: Background process finished with pid %d\n", wc);
				}
			}
			period++;
		
			// Freeing the allocated memory	
			for(i=0;tokens[i]!=NULL;i++){
				free(tokens[i]);
			}
			free(tokens);

			__multisize = 0;
			ismulti = false;
		}
	}

	// In the end free up everything the memorie's taken and exit the shell.
	for (i = 0; i < MAX_MULTI_COMMANDS; i++) {
		if (__multiexec[i] != NULL) {
			for (int j = 0; __multiexec[i][j] != NULL; j++) {
				free(__multiexec[i][j]);
			}
			free(__multiexec[i]);
		}
	}
	return 0;
}
