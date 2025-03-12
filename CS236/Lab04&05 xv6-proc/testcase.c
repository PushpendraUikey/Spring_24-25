#include "types.h"
#include "stat.h"
#include "user.h"

// #################### Part C #################

// void welcome(void)
// {
//   printf(1, "I am child in welcome function\n");
//   welcomeDone();
// }

// int main(int argc, char *argv[])
// {

//   int ret1 = fork();
//   if(ret1 == 0)
//     {
//       printf(1, "I am child with no welcome\n"); 
//     }
//   else if (ret1 > 0)
//     {
//       wait();
//       printf(1, "Parent reaped first child\n");
     
//       welcomeFunction(&welcome);
//       int x = 5;
//       int ret2 = fork();
//       if (ret2 == 0)
// 	{
//     int y = 5;
//     printf(1, "x + y = %d\n", x+y);
// 	  printf(1, "I have returned from welcome function\n");
// 	  exit();
// 	}
//       else if (ret2 > 0)
// 	{
// 	  wait();
// 	  printf(1, "Parent reaped second child\n");
// 	}
//     }
  
//   exit();
// }


// ########################## Part B #########################

void delay(int count)
{
    int i;
    int j, k;
    int *data;

    data = (int *)malloc(sizeof(int) * 1024 * 10);
    if (data <= 0)
        printf(1, "Error on memory allocation \n");

    for (i = 0; i < count; i++)
    {
        for (k = 0; k < 5700; k++)
            for (j = 0; j < 1024 * 10; j++)
                data[j]++;
    }
}
int main(int argc, char *argv[])
{

  int N = 5; //5 children, please change and test with other values also

  int pids[N];
  int rets[N];
  setprio(1000);
  printf(1, "Priority of parent process = %d\n", getprio());

  for (int i = 0; i < N; i++)
    {
      // Set process priority
      // Change priority of children in different order
      // and verify your implementations
      // this example sets higher priority to later children
      // so you should expect children to exit in reverse order of their creation
      
      int priority = 10*i + 1 ;
      
      int ret = fork();
      if (ret == 0)
        {
	  setprio(priority);
	  delay(20);
    // sleep(8);
	  exit();
        }
      else if (ret > 0)
        {
	  pids[i] = ret;
        }
      else
        {
	  printf(1, "fork error \n");
	  exit();
        }
    }
  
  for (int i = 0; i < N; i++)
    {
      rets[i] = wait();
    }
  
  printf(1, "\nAll children completed\n");
  for (int i = 0; i < N; i++)
    printf(1, "Child %d.    pid %d\n", i, pids[i]);
  
  printf(1, "\nExit order \n");
  for (int i = 0; i < N; i++)
    printf(1, "pid %d\n", rets[i]);
  
  exit();
}
