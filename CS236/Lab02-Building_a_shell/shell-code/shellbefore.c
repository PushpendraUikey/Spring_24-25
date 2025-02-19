#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

// This version was stored as a backup to the original 

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
// Token generator
char **tokenize(char *line)
{				 // malloc() returns a pointer type-cast it!
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));	// have to clean the memory manually
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
  tokens[tokenNo] = NULL ;	// Null terminated array
  return tokens;
}

void handle_sigint(int sig){
    (void)sig; // Suppress unused parameter warning
	// do nothing just catch the ctrl+c
}

int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;
	char **__multiexec[MAX_NUM_TOKENS];
	int __multisize = 0;   
	int i;

	int period = 0;
	signal(SIGINT, handle_sigint);	// Suppressing the signal cntrl+c
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
		scanf("%[^\n]", line);//Reads all charachters until a newline is encountered.
		getchar();// this effectively reads the last newline charachter.

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
			// -1 in place of pid: any process waiting to be reaped(i.e. it is in zombie state gets reaped)
			// WNOHANG: it actually allows waitpid to not wait for any child to become zombie but it 
			// immediately returns zero if no process is in zombie state.
			//while ((wc = waitpid(-1, NULL, WNOHANG)) > 0);
            while ((wc = wait(NULL)) > 0) {	// there's no direct method to kill all the child unless you know their pid
                printf("Shell: Background process finished with pid %d\n", wc);
            }
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
				k = -1;
			}else{
				__multiexec[__multisize][k] = tokens[lastIndex];
			}
			k++;
		}

        lastIndex--; 					// Move to the last token
        if (strcmp(tokens[lastIndex], "&") == 0) {
            runInBackground = 1;
            free(tokens[lastIndex]);
            tokens[lastIndex] = NULL; 	// Remove '&' from the tokens(Null Termination)
        }

		// cd command processing
		if (strcmp(tokens[0], "cd") == 0) {
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

	}
	return 0;
}
