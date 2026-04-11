#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int pids[10];
  int n, i, status;

  printf("testproc: forking 3 children\n");

  // Fork 3 children
  for(i = 0; i < 3; i++){
    int pid = fork();
    if(pid == 0){
      pause(2);   // children sleep briefly
      printf("child %d exiting\n", getpid());
      exit(i + 10);  // exit with status 10, 11, 12
    }
  }

  // getchildren: list living children
  n = getchildren(pids, 10);
  printf("getchildren returned %d children: ", n);
  for(i = 0; i < n && i < 10; i++)
    printf("%d ", pids[i]);
  printf("\n");

  // waitpid: wait for each child specifically
  n = getchildren(pids, 10);
  for(i = 0; i < n && i < 10; i++){
    int ret = waitpid(pids[i], &status);
    printf("waitpid(%d) returned %d, exit status = %d\n",
           pids[i], ret, status);
  }

  printf("testproc done\n");
  exit(0);
}