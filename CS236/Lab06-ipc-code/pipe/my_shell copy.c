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

void free_mem(char **tokens){
    // To free the memory taken by these strings
	for(int i = 0 ; tokens[i] != NULL ; i++ ) {
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
	bool XX__isparll = false;
	int i , j;
	int wc, rc;
	int period = 0;

	signal(SIGINT, handle_sigint);	// This also preventing the our shell to not exit
	while(1) {			
		/* BEGIN: TAKING INPUT */

		bzero(line, sizeof(line));
		char *cwd = getcwd(NULL, 0); 	// Dynamically allocate cwd[if args given the allocate into the buffer]
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
			// kill(0, SIGTERM);// Not good as terminates entire group of current process including self.
			// -1 in place of pid in waitpid: any process waiting to be reaped
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
			if( !XX__isparll && strcmp(tokens[lastIndex], "&&")==0){
				XX__multiexec[XX__multisize++][k] = NULL;
				XX__ismulti = true;
				k = 0;
			}
			else if( !XX__ismulti && strcmp(tokens[lastIndex], "&&&")==0){
				XX__multiexec[XX__multisize++][k] = NULL;
				XX__isparll = true;
				k = 0;
			}
			else{
				XX__multiexec[XX__multisize][k] = strdup(tokens[lastIndex]);
				k++;
			}
		}
		XX__multiexec[XX__multisize++][k] = NULL;		// Last one make NULL terminated

		if(XX__ismulti && XX__isparll){			// can't have both type of execution
			// Reset
			free_mem(tokens);
			XX__multisize = 0;
			XX__ismulti = false;
			XX__isparll = false;
			continue;
		}

		// ######################### && #########################
		if(XX__ismulti){
			for(j=0; j<XX__multisize; j++){
				// cd command processing
				if (XX__multiexec[j][0] != NULL && strcmp(XX__multiexec[j][0], "cd") == 0) {
					if (XX__multiexec[j][1]== NULL || XX__multiexec[j][2] != NULL) {
						printf("Invalid directory change arguments\n");
					} else if (chdir(XX__multiexec[j][1]) < 0) {
						perror("Error: No such directory exists.\n"); 
					}
					continue;
				}

				rc = fork();
				if(rc==0){
					// if(runInBackground) signal(SIGINT, handle_sigint); //if it is a background process then suppress ctrl+c
					execvp(XX__multiexec[j][0], XX__multiexec[j]);
					printf("%s: Invalid command!\n", XX__multiexec[j][0]);
					exit(EXIT_FAILURE);
				}else if(rc>0){
					// each parent process has to wait for the child to be reaped.
					wc = waitpid(rc, NULL, 0);
				}
			}

			// Reset
			free_mem(tokens);
			XX__ismulti = false;
			XX__multisize = 0;
		}

		// ########################### &&& ##########################
		else if(XX__isparll)
				// parallel execution here.
		{
			int childCount = 0;
			for(j=0; j<XX__multisize; j++){
				// cd command processing
				if (XX__multiexec[j][0] != NULL && strcmp(XX__multiexec[j][0], "cd") == 0) {
					if (XX__multiexec[j][1]== NULL || XX__multiexec[j][2] != NULL) {
						printf("Invalid directory change arguments\n");
					} else if (chdir(XX__multiexec[j][1]) < 0) {
						perror("Error: No such directory exists.\n"); 
					}
					continue;
				}

				rc = fork();
				if(rc==0){
					// if(runInBackground) signal(SIGINT, handle_sigint); //if it is a background process then suppress ctrl+c
					execvp(XX__multiexec[j][0], XX__multiexec[j]);
					printf("%s: Invalid command!\n", XX__multiexec[j][0]);
					exit(EXIT_FAILURE);
				}else if(rc>0){
					childCount++;
				}
			}

			while(childCount--){
				wc = wait(NULL);	// wait for all the childrens to be reaped.
			}
			
			// reset
			free_mem(tokens);
			XX__multisize = 0;
			XX__isparll = false;	// Reset for new command
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
				if (tokens[1] == NULL || tokens[2] != NULL) {
					printf("Invalid directories given.\n");
					free_mem(tokens);
				} else if (chdir(tokens[1]) < 0) {
					printf("Error: <%s> No such directory exists.\n", tokens[1]); 
				}
				continue;
			}

			rc = fork();
			if(rc==0){
				// if(runInBackground) signal(SIGINT, handle_sigint);// if it is a background process then suppress cntrl+c
				// since new mem image is being created thus default signal handling behavior described won't
				// be applicable after exec hence this ignore signal method is used which is copied on exec
				if(runInBackground){
					signal(SIGINT, SIG_IGN);	// background process ignore the SIGINT signal
				}
				execvp(tokens[0], tokens);
				printf("%s: Invalid command!\n", tokens[0]);
				exit(EXIT_FAILURE);
			}else if(rc>0){
				if(!runInBackground){		// if running in the background then don't wait!
					int wc = waitpid(rc, NULL, 0);	// foreground processes are reaped here
				}
			}


			if(period%3==0){			// reaping the zombie processess periodically
			// WNOHANG: it actually allows waitpid to not wait for any child to become zombie but it 
			// immediately returns zero if no process is in zombie state. [ i.e. it won't wait for the processes which are running in the background]
				if((wc = waitpid(-1, NULL, WNOHANG)) > 0) {	
					printf("Shell: Background process reaped with pid %d\n", wc);
				}
			}
			period++;
		
			// Reset
			free_mem(tokens);
			XX__multisize = 0;
			XX__ismulti = false;
			XX__isparll = false;
		}
	}

	// In the end free up everything and exit the shell.
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


// This can be implemented if we use a Data Structure to hold the pid's of each child processes
// We have to have maxchild number to store in the fixed sized array.

//  if (strcmp(tokens[0], "exit") == 0) {
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


// kill(-PID, SIGNAL)	Sends signal to all processes in group PID.
// kill(-1, SIGNAL)	Sends signal to all processes except init and restricted ones.
// kill(0, SIGNAL)	Sends signal to all processes in the caller's group.
