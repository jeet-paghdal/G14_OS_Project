#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/procinfo.h"
#include "user/user.h"

int main(void)
{
  int count = getproccount();
  printf("Active process count: %d\n", count);

  int mypid = getpid();
  struct procinfo info;

  if(getprocinfo(mypid, &info) == 0){
    printf("procinfo for pid %d:\n", mypid);
    printf("  name  : %s\n", info.name);
    printf("  state : %d\n", info.state);
    printf("  size  : %d bytes\n", info.sz);
  } else {
    printf("getprocinfo failed\n");
  }

  // Try an invalid pid
  if(getprocinfo(9999, &info) < 0)
    printf("getprocinfo(9999) correctly returned -1\n");

  exit(0);
}