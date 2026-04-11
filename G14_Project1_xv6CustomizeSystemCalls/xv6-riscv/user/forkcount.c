#include "kernel/types.h"
#include "user/user.h"

int main() {
    for(int i = 0; i < 20; i++) {
        int pid = fork();
        if(pid < 0) {
            printf("Fork failed at iteration %d\n", i);
            break;
        }
        if(pid == 0) exit(0);
    }
    wait(0);
    exit(0);
}