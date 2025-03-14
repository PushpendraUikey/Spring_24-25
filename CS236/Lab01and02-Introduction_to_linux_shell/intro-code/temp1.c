#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

int main(int argc, char* argv[]){
	int rc = fork();

	if(rc==0){
		printf("Hello, I am child (pid:%d)\n", (int)getpid());
		char* myargs[3];
		myargs[0] = strdup("ls");
		myargs[1] = strdup("-l");
		myargs[2] = NULL;
		execvp(myargs[0], myargs);
		printf("If reached here, means failed");
		exit(1);
	}else if(rc>0){
		int wc = wait(NULL);
		printf("Hello I am the parent of %d (pid:%d)\n", wc, (int)getpid());
	}
}
	
