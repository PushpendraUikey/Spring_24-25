/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 *
 * Figure 3.17
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#define ITERATION 512

int main()
{
	const int SIZE = 4096;		// 4 KB shared memory segement.
	const char *name = "OS";
	const char *empty = "freeeee";
	const char *message= "OSisFUN";
	const char *myfifo = "/tmp/myfifo";

	int shm_fd;
	int fifo_fd;
	void *ptr, *bsptr;

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);	// file descriptor!

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	bsptr = ptr;
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	mkfifo(myfifo, 0666);		// Named pipe creation

	/**
	 * Now write to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */
	// Writing on the shared image by producer!
	for(int i=1; i<= 512; i++){
		sprintf(ptr, "%s", empty);
		ptr += strlen(empty);
	}
	ptr = bsptr;
	int offset = 0;		// Progammar has to make sure offset within [0,512]
	int length_str = strlen(message);
	while(1){
		// The below code is incorrect as incorrectly scaling the range causing segfault as inapproriate mem access
		// Pointer Arithmetic: per increment in pointer moves to size of (pointer's datatype) here char.
		// char * offset_ptr = (char* )bsptr + offset * length_str;
		char * offset_ptr = (char* )bsptr + offset;		// This is correct
		sprintf(offset_ptr, "%s", message);	// writing on the shared page(memory)
		
		// Opening pipe to write into it
		fifo_fd = open(myfifo, O_WRONLY);
		write(fifo_fd, &offset, sizeof(int));	// passby reference, argument requires pointer
		close(fifo_fd);
		
		// now read the message 
		fifo_fd = open(myfifo, O_RDONLY);
		read(fifo_fd, &offset, sizeof(int));
		close(fifo_fd);

		if(offset == -1) break;	// breaking condition
	}

	return 0;
}

