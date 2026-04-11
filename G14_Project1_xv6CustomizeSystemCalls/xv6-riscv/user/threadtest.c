#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void thread_func(void *arg) {
    printf("Child thread: arg = %d\n", *(int*)arg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int arg = 42;
    int pid;

    printf("Parent: calling thread_create\n");
    pid = thread_create(thread_func, &arg);
    if (pid < 0) {
        printf("thread_create failed\n");
        exit(1);
    }
    printf("Parent: thread_create returned pid %d\n", pid);
    printf("Parent: continuing execution\n");

    // Wait for child to finish
    wait(0);
    printf("Parent: child finished\n");

    exit(0);
}