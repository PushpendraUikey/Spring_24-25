/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process
 *
 * @author Pushpendra Uikey
 * Operating System Lab 06
 * Spring Semester: 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#define ITERATION 512

int main()
{
	const char *name = "OS";
	const int SIZE = 4096;
	const char *empty = "freeeee";
	const char *myfifo = "/tmp/myfifo";

	int shm_fd;
	int fifo_fd;
	void *ptr;
	int i;

	/* open the shared memory segment */
								// 0:Octal(base 8), 6:rw, 6:rw, 6:rw
	shm_fd = shm_open(name, O_RDWR, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the address space of the process */
	// mmap returns the starting address of shared memory in the process.
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	mkfifo(myfifo, 0666);	// creating the pipe.

	int message_len = strlen(empty);
	int offset;

	for(i=1; i<=1000; i++){

		fifo_fd = open(myfifo, O_RDONLY);
		read(fifo_fd, &offset, sizeof(int));
		close(fifo_fd);


		char * offset_ptr = (char *) ptr + offset;

		printf("Received %d: %.*s\n", i, message_len, offset_ptr);

		// Write back to indicate free memory
		sprintf(offset_ptr, "%s", empty);
		sleep(0.2);

		// Return the signal to the producer
		offset = (offset + (i * message_len)) % ITERATION;	// make the offset bound
		fifo_fd = open(myfifo, O_WRONLY);
		write(fifo_fd, &offset, sizeof(int));
		close(fifo_fd);
	}
	// Read to flush the pipe 
	fifo_fd = open(myfifo, O_RDONLY);
	read(fifo_fd, &offset, sizeof(int));
	close(fifo_fd);
	// sending the signal to terminate.
	offset = -1;
	fifo_fd = open(myfifo, O_WRONLY);
	write(fifo_fd, &offset, sizeof(int));
	close(fifo_fd);

	/* remove the shared memory segment */
	if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}

	return 0;
}

// shm_fd is just a reference to the shared memory object.
// It is used by mmap() to create a mapping.
// After mmap(), shm_fd is no longer needed for direct access since ptr provides direct access to shared memory.