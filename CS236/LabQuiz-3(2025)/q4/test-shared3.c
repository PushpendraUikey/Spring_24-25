#include "types.h"      
#include "user.h"

int main(int argc, char *argv[]) 
{ 
  uint *ap = (uint *) mapshared();
  int pid = fork();

  if(pid == 0) {
    sleep(2); //wait for parent to write
    uint *ac = (uint *) getshared();
    printf(1, "child %d\n", *ac);
    *ac = 4567;
    int pid2 = fork();
    if (pid2 == 0)
    {
        uint *agc = (uint *) getshared();
        printf(1, "grandchild %d\n", *agc);
        *agc = 1340;
    }
    else {
        sleep(10);
        printf(1, "child again %d\n", *ac);
        *ac = 5431;
        sleep(20); //sleep to give parent time to unmap
        wait();
    }
    
  }

  else {
    *ap = 4211;
    sleep(5); //wait for child to reply
    printf(1, "parent %d\n", *ap);
    *ap = 4309; //write again
    sleep(10);
    printf(1, "parent again %d\n", *ap);
    if(unmapshared() < 0)
      printf(1, "could not unmap shared page\n");
    wait();
  }
  
  exit();
}
