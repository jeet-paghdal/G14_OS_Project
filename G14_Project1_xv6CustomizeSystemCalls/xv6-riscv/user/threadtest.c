#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void delay(int ticks) {
    for (volatile int i = 0; i < ticks * 100000; i++) {
        // busy wait
    }
}

void thread_func(void *arg) {
    delay(1);  // small delay before printing
    printf("Child thread: arg = %d\n", *(int*)arg);
    delay(2);  // delay before exit
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
    delay(1);  // delay after thread_create call
    printf("Parent: thread_create returned pid %d\n", pid);
    delay(1);  // delay before continuing
    printf("Parent: continuing execution\n");
    delay(1);  // delay before wait
    // Wait for child to finish
    wait(0);
    printf("Parent: child finished\n");

    exit(0);
}