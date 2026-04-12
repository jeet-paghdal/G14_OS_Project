# Project 1: xv6 Customize System Calls
  
**Group:** G14  

## Overview
This repository contains the implementation for Project 1, which focuses on analyzing and modifying the xv6 operating system by adding new system calls. The project demonstrates core OS concepts including process creation, inter-process communication, and signals.

## Contributors
* **Nidhi Mithiya (24JE0664)** - 
* **Nilay Choudhary (24JE0665)** - Signals Implementation
* **PR Sanjit Ram (24JE0666)** - 
* **Paghdal Jeet Prakashkumar (24JE0667)** - 
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

## Feature 2: 
**Implemented by:**

[Details to be added]

---

## Feature 3: 
**Implemented by:** 

[Details to be added]