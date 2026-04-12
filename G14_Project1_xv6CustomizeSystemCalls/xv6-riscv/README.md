# Project 1: xv6 Customize System Calls
  
**Group:** G14  

## Overview
This repository contains the implementation for Project 1, which focuses on analyzing and modifying the xv6 operating system by adding new system calls. The project demonstrates core OS concepts including process creation, inter-process communication, and signals.

## Contributors
* **Nidhi Mithiya (24JE0664)** - 
* **Nilay Choudhary (24JE0665)** - Signals Implementation
* **PR Sanjit Ram (24JE0666)** - 
* **Paghdal Jeet Prakashkumar (24JE0667)** - `waitpid` & `getchildren` Implementation
* **Parul Sharma (24JE0669)** - 
* **Patel Het Alpeshbhai (24JE0670)** - 

---

## Feature 1: Signal Handling
**Implemented by:** Nilay Choudhary

This feature introduces a software interrupt mechanism allowing asynchronous communication between processes in xv6.

### Core Modifications
* **Process Control Block (`kernel/proc.h`)**: Expanded `struct proc` to include:
  * `pending_signals`: A 32-bit integer acting as a bitmap for pending signals.
  * `signal_handlers[32]`: An array storing user-space memory addresses for registered signal handlers (initialized to `-1` to handle xv6's `0x0` virtual address baseline).
  * `saved_trapframe`: A backup of the process CPU state to allow resumption after signal execution.
  * `handling_signal`: A lock flag to prevent nested signal corruption.

### System Calls Implemented
1. `sys_signal(int signum, void (*handler)())`
   * **Purpose:** Registers a custom function as the handler for a specific signal number.
   * **Mechanism:** Saves the function's memory address into the `signal_handlers` array of the current process.

2. `sys_sigsend(int pid, int signum)`
   * **Purpose:** Delivers a signal from one process to another.
   * **Mechanism:** Iterates through the process table to find the target `pid`, acquires its lock, and flips the corresponding bit in its `pending_signals` bitmap using a bitwise OR operation.

3. `sys_sigreturn(void)`
   * **Purpose:** Restores the process state after a signal handler completes.
   * **Mechanism:** Copies the `saved_trapframe` back into the main `trapframe` and resets the `handling_signal` flag, ensuring the process resumes its main execution loop safely.

### Trap Routing (`kernel/trap.c`)
The signal execution logic is injected into `usertrap()` immediately before `prepare_return()`. If a signal is pending and a valid handler is registered (not equal to `-1`), the kernel backs up the current trapframe, modifies the Exception Program Counter (`epc`) to point to the user's custom handler, and allows the CPU to jump to that function upon returning to user space.

### Testing & Execution
* **Test Program:** `$ testsignals`
* **Screenshots:** The execution proof demonstrating the successful registration, delivery, execution, and resumption of the signal loop can be found in the `screenshots/` directory.

---

## Feature 2: `waitpid` and `getchildren` System Calls
**Implemented by:** Paghdal Jeet Prakashkumar (24JE0667)

This feature introduces two new process management system calls that extend xv6's existing `wait()` functionality with more precise control over child process synchronization and inspection.

### Core Modifications
* **Kernel Implementations (`kernel/proc.c`)**: Two new kernel functions were added:
  * `kwaitpid`: Searches the global `proc[]` table for a child matching a specific PID and blocks until that child becomes a ZOMBIE, then collects its exit status and frees its resources via `freeproc()`.
  * `kgetchildren`: Scans the `proc[]` table for all living (non-ZOMBIE, non-UNUSED) children of the calling process and copies their PIDs into a user-space buffer using `copyout()`.

* **Kernel Internal Declarations (`kernel/defs.h`)**: Added prototypes for `kwaitpid(int, uint64)` and `kgetchildren(uint64, int)` in the `proc.c` section so other kernel files can reference them.

* **Syscall Dispatch Table (`kernel/syscall.c`)**: Registered both new syscall handler functions (`sys_waitpid`, `sys_getchildren`) in the `extern` declaration list and in the `syscalls[]` function pointer array at indices 22 and 23.

* **Syscall Numbers (`kernel/syscall.h`)**: Assigned unique numbers:
  * `SYS_waitpid     22`
  * `SYS_getchildren 23`

* **Argument Extraction Layer (`kernel/sysproc.c`)**: Added `sys_waitpid()` and `sys_getchildren()` which read arguments from the process trapframe registers using `argint()` and `argaddr()`, then delegate to the kernel implementations.

* **User-Space Stubs (`user/usys.pl`)**: Added `entry("waitpid")` and `entry("getchildren")` to auto-generate the assembly stubs that load the syscall number into register `a7` and execute the `ecall` instruction.

* **User-Space Interface (`user/user.h`)**: Declared the public prototypes:
  ```c
  int waitpid(int pid, int *status);
  int getchildren(int *buf, int maxn);
  ```

### System Calls Implemented

1. `sys_waitpid(int pid, int *status)`
   * **Purpose:** Waits for a specific child process (identified by `pid`) to exit, rather than waiting for any child like the existing `wait()`.
   * **Mechanism:** Acquires `wait_lock` and loops through the process table searching for a child whose `pid` matches and whose `parent` is the calling process. If the child is already a ZOMBIE, it uses `copyout()` to safely write the exit status to the user-space `status` pointer, calls `freeproc()` to clean up the child's resources, and returns the pid. If the child is still running, it calls `sleep(p, &wait_lock)` to block until any child exits (via `wakeup` triggered inside `kexit()`), then re-checks in a loop.
   * **Return Value:** The waited-on `pid` on success, `-1` if the pid is not a child of the caller or on error.

2. `sys_getchildren(int *buf, int maxn)`
   * **Purpose:** Returns the PIDs of all currently living child processes of the calling process into a user-provided array.
   * **Mechanism:** Acquires `wait_lock` and iterates the entire `proc[]` table. For each entry where `pp->parent == current_process` and the state is neither `UNUSED` nor `ZOMBIE`, the PID is stored in a local kernel buffer. After releasing the lock, `copyout()` transfers up to `maxn` entries to the user-space buffer `buf`.
   * **Return Value:** The total count of living children (may exceed `maxn` if the buffer was too small), or `-1` on a bad user pointer.


### Testing & Execution
* **Test Program:** `$ testproc`
* **What the test does:**
  1. Forks 3 child processes, each sleeping briefly before exiting with a distinct status code (10, 11, 12).
  2. Calls `getchildren()` immediately after forking to capture and print all living child PIDs.
  3. Calls `waitpid()` for each child individually, printing the returned PID and collected exit status.
* **Expected Output:**
  ```
  testproc: forking 3 children
  getchildren returned 3 children: 4 5 6
  child 4 exiting
  child 5 exiting
  child 6 exiting
  waitpid(4) returned 4, exit status = 10
  waitpid(5) returned 5, exit status = 11
  waitpid(6) returned 6, exit status = 12
  testproc done
  ```
* **Screenshots:** Execution proof demonstrating successful child listing via `getchildren` and precise per-PID synchronization via `waitpid` can be found in the `screenshots/` directory.

---

## Feature 3: 
**Implemented by:** 

[Details to be added]