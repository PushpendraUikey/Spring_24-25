#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char* argv[]){
	int rc = fork();

	if(rc == 0){
		printf("Hello, I am child with (pid:%d)\n", (int)getpid());
		exit(1);
	}else if(rc>0){
		int wc = wait(NULL);
		printf("Hello, I am parent of %d (wc:%d) (pid:%d) \n", rc, wc, (int) getpid());
	}
}
