#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define FNAME_SIZE 100
#define MAX_FILE_NO 10000
#define BLOCK_SIZE 1024

// fopen, flose : -- works with the FILE* structure.
// open, close : -- works with the file descriptor.

int main(int argc, char *argv[])
{

  int n, file_no;
  FILE *fp;
  char dest_file_name[FNAME_SIZE];
  char buf[BLOCK_SIZE];

  while(1)
    {
      //file_no = rand() % MAX_FILE_NO;
      file_no = 0;
      
      // sets all the bytes of dest_file_name to zero
      bzero(dest_file_name, FNAME_SIZE);
      sprintf(dest_file_name, "disk-files/foo%d.pdf", file_no);

      fp = fopen(dest_file_name, "rb");
      if (fp == NULL) {
        perror("Can't open dest file");
        exit(1);
      }

      // sets all the bytes of 'buf' to zero
      bzero(buf,BLOCK_SIZE);
      while  ( (n = (int)fread( buf, 1, BLOCK_SIZE, fp )) > 0)
      {
        //do nothing with the read data;
        bzero(buf,BLOCK_SIZE);
      }

       fclose(fp);
    }
}

// bzero: is the function which sets all the bytes of the destination memory to zero.
// It is similar to the memset(<dest>, <content>, <block-size>); ----> #define bzero(s,n) memset((s), 0, (n))


// Level of Abstraction:
// fread() is part of the C standard I/O library (stdio.h). It works with a FILE * stream, 
// which includes buffering.
// read() is a low-level system call (declared in unistd.h) that works with file descriptors 
// and performs unbuffered I/O.

// Buffering:
// fread() automatically buffers data, reducing the number of system calls when reading small chunks of data.
// read() directly interacts with the operating system, reading raw data from the file descriptor 
// without additional buffering.

// Usage and Parameters:
// With fread(), you specify the size and number of elements to read (e.g., fread(buf, 1, BLOCK_SIZE, fp)), 
// and it returns the number of elements read.
// With read(), you specify the number of bytes to read (e.g., read(fd, buf, BLOCK_SIZE)), and it returns 
// the number of bytes actually read.