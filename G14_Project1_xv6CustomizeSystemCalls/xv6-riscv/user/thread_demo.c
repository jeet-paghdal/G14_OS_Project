#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void delay(int ticks) {
    for (volatile int i = 0; i < ticks * 100000; i++) {
        // busy wait
    }
}

struct task {
    int id;
    char* name;
};

void worker_thread(void* arg) {
    struct task* t = (struct task*)arg;
    printf("[Thread %d] Starting work on: %s\n", t->id, t->name);
    delay(3);
    printf("[Thread %d] Finished processing: %s\n", t->id, t->name);
    thread_exit();
}

int main(int argc, char *argv[]) {
    struct task tasks[3] = {
        {1, "task_A"},
        {2, "task_B"},
        {3, "task_C"}
    };
    
    int pids[3];
    
    printf("=== Thread Demo: Concurrent Task Processing ===\n\n");
    
    // Create multiple worker threads
    printf("Main: Creating 3 worker threads...\n");
    for (int i = 0; i < 3; i++) {
        pids[i] = thread_create(worker_thread, &tasks[i]);
        if (pids[i] < 0) {
            printf("Main: Failed to create thread %d\n", i);
            exit(1);
        }
        printf("Main: Created thread with PID %d for task %s\n", pids[i], tasks[i].name);
        delay(1);
    }
    
    printf("\nMain: All threads created. Waiting for them to complete...\n");
    delay(2);
    
    // Wait for each thread to complete
    for (int i = 0; i < 3; i++) {
        printf("Main: Waiting for thread %d (PID %d) to complete...\n", i + 1, pids[i]);
        thread_join(pids[i]);
        printf("Main: Thread %d has completed!\n\n", i + 1);
        delay(1);
    }
    
    printf("=== All threads completed successfully ===\n");
    printf("Main: Exiting program\n");
    exit(0);
}
