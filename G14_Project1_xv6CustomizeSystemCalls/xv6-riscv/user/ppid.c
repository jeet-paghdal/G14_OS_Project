#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
    int pid = getpid();
    int ppid = getppid();

    printf("My PID: %d\n", pid);
    printf("My Parent PID: %d\n", ppid);

    exit(0);
}