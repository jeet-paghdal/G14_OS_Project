#ifndef PROCINFO_H
#define PROCINFO_H

#define PROC_NAME_MAX 16

struct procinfo {
  int pid;
  int state;       // 0=UNUSED,1=USED,2=SLEEPING,3=RUNNABLE,4=RUNNING,5=ZOMBIE
  int sz;          // memory size in bytes
  char name[PROC_NAME_MAX];
};

#endif