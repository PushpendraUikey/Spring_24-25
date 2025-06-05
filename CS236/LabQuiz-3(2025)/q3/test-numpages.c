#include "types.h"      
#include "user.h"

int main(int argc, char *argv[]) 
{ 
  printf(1, "getnumpages %d\n", getnumpages());
  char *x = sbrk(4096*5);
  if(x == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }
  printf(1, "getnumpages %d\n", getnumpages());

  x = sbrk(4096*1024*2);
  if(x == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }
  printf(1, "getnumpages %d\n", getnumpages());
  
  exit();
}
