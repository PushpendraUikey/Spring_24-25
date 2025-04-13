#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void)
{
  int globlval = 8;
  printf(1, "free pages available: %d\n", getNumFreePages());
  int ret = fork();
  if(ret == 0){
    printf(1, "free pages available after fork in child: %d\n", getNumFreePages());
    globlval += 5;
    sleep(1);
    printf(1, "free pages after writing %d\n", getNumFreePages());
  }else if(ret > 0){
    printf(1, "free pages available after fork in parent: %d\n", getNumFreePages());
    wait();
    globlval += 5;
    printf(1, "free pages after child done %d\n", getNumFreePages());
  }
  exit();
}
