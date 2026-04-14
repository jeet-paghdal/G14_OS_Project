#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/procinfo.h"
#include "user/user.h"

int
main(void)
{
  int pid;
  int ppid;
  int count;
  char msg[64];
  struct procinfo info;

  count = getproccount();
  printf("========================================\n");
  printf("        xv6 IPC Demonstration\n");
  printf("========================================\n");
  printf("Active processes before test: %d\n", count);
  printf("Parent process PID: %d\n", getpid());

  pid = fork();
  if(pid < 0){
    printf("fork failed\n");
    exit(1);
  }

  if(pid == 0){
    ppid = getppid();
    printf("\n[child] PID %d started\n", getpid());
    printf("[child] Parent PID: %d\n", ppid);
    if(getprocinfo(getpid(), &info) == 0){
      printf("[child] Proc info: name=%s state=%d size=%d bytes\n",
             info.name, info.state, info.sz);
    }

    printf("[child] Waiting for parent message...\n");
    if(ipc_recv(msg, sizeof(msg)) < 0){
      printf("child: ipc_recv failed\n");
      exit(1);
    }

    printf("[child] Received message: \"%s\"\n", msg);
    printf("[child] Message length: %d bytes\n", strlen(msg));

    printf("[child] Sending reply back to parent...\n");
    if(ipc_send(getppid(), "reply from child", 17) < 0){
      printf("child: ipc_send failed\n");
      exit(1);
    }

    printf("[child] Reply sent successfully\n");

    exit(0);
  }

  pause(10);

  printf("\n[parent] PID %d spawned child PID %d\n", getpid(), pid);
  if(getprocinfo(pid, &info) == 0){
    printf("[parent] Child snapshot: name=%s state=%d size=%d bytes\n",
           info.name, info.state, info.sz);
  }

  printf("[parent] Sending message to child...\n");
  if(ipc_send(pid, "hello from parent", 18) < 0){
    printf("parent: ipc_send failed\n");
    exit(1);
  }

  printf("[parent] Waiting for reply from child...\n");
  if(ipc_recv(msg, sizeof(msg)) < 0){
    printf("parent: ipc_recv failed\n");
    exit(1);
  }

  printf("[parent] Received reply: \"%s\"\n", msg);
  printf("[parent] Reply length: %d bytes\n", strlen(msg));

  wait(0);

  printf("\n========================================\n");
  printf("IPC test completed successfully\n");
  printf("========================================\n");
  exit(0);
}