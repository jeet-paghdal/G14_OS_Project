#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"
#include "procinfo.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0;  // not reached
}

uint64
sys_getproccount(void)
{
  return kgetproccount();
}

uint64
sys_getprocinfo(void)
{
  int pid;
  uint64 addr;
  struct procinfo info;

  argint(0, &pid);
  argaddr(1, &addr);

  if(kgetprocinfo(pid, &info) < 0)
    return -1;

  if(copyout(myproc()->pagetable, addr, (char*)&info, sizeof(info)) < 0)
    return -1;

  return 0;
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_thread_create(void)
{
  uint64 func, arg;
  argaddr(0, &func);
  argaddr(1, &arg);
  return thread_fork(func, arg);
}

uint64
sys_thread_join(void)
{
  int pid;
  argint(0, &pid);
  return kwaitpid(pid, 0);
}

uint64
sys_thread_exit(void)
{
  kexit(0);
  return 0;  // not reached
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_getppid(void)
{
  struct proc *p = myproc();
  return p->parent ? p->parent->pid : 0;
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    if(addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_waitpid(void)
{
  int pid;
  uint64 addr;
  argint(0, &pid);
  argaddr(1, &addr);
  return kwaitpid(pid, addr);
}

uint64
sys_getchildren(void)
{
  uint64 addr;
  int maxn;
  argaddr(0, &addr);
  argint(1, &maxn);
  return kgetchildren(addr, maxn);
}

uint64
sys_signal(void)
{
  int signum;
  uint64 handler;

  // Just fetch the arguments directly, no if-check needed
  argint(0, &signum);
  argaddr(1, &handler);
    
  if(signum < 0 || signum >= 32)
    return -1;

  myproc()->signal_handlers[signum] = handler;

  return 0;
}

uint64
sys_sigreturn(void)
{
  struct proc *p = myproc();
  
  // Restore the original trapframe to resume normal execution
  *(p->trapframe) = p->saved_trapframe;
  p->handling_signal = 0;
  
  return p->trapframe->a0;
}

uint64
sys_sigsend(void)
{
  int pid, signum;
  
  // Fetch arguments directly
  argint(0, &pid);
  argint(1, &signum);
    
  if(signum < 0 || signum >= 32)
    return -1;

  // ksigsend needs to be implemented in proc.c
  return ksigsend(pid, signum);
}