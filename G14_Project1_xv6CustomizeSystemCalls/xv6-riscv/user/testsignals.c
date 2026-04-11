#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// A flag to tell the main loop we got the signal
// (Using volatile so the compiler doesn't optimize the loop away)
volatile int signal_received = 0;

// The custom signal handler function
void my_handler(void) {
  printf("\n>>> HANDLER FIRED: Signal received! <<<\n\n");
  signal_received = 1; // Flip the flag
  
  // CRITICAL: Must call sigreturn to restore the trapframe and resume normal execution
  sigreturn(); 
}

int main(int argc, char *argv[]) {
  int pid;
  int signum = 10; // Arbitrary signal number between 0 and 31

  printf("Starting signal test...\n");

  pid = fork();

  if (pid < 0) {
    printf("Fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    // --- CHILD PROCESS ---
    printf("Child (PID %d): Registering handler for signal %d\n", getpid(), signum);
    signal(signum, my_handler);

    printf("Child (PID %d): Waiting for signal in a loop...\n", getpid());
    
    // Sit in a tight loop until the signal handler flips the flag
    while (signal_received == 0) {
      // Busy waiting
    }

    printf("Child (PID %d): Successfully resumed main loop after handling signal!\n", getpid());
    exit(0);
    
  } else {
    // --- PARENT PROCESS ---
    // Give the child a moment to register its handler
    // Your xv6 has a pause(ticks) function acting as a sleep
    pause(10); 

    printf("Parent: Sending signal %d to child PID %d\n", signum, pid);
    sigsend(pid, signum);

    // Wait for the child to finish and exit cleanly
    wait(0);
    printf("Parent: Child finished. Signal test complete.\n");
    exit(0);
  }
}