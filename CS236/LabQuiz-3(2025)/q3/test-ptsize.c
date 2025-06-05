#include "types.h"      
#include "user.h"

int main(int argc, char *argv[]) 
{ 
  printf(1, "getptsize %d\n", getptsize());
  char *x = sbrk(4096*7);
  if(x == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }
  printf(1, "getptsize %d\n", getptsize());

  x = sbrk(4096*1024*5);
  if(x == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }
  printf(1, "getptsize %d\n", getptsize());
  
  exit();
}
