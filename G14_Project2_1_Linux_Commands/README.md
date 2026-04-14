# Project 2: Custom UNIX-Like Shell and Utilities

**Group:** G14  

## Overview
This repository contains the implementation for Project 2 (Option 1). We have developed a custom, lightweight UNIX-like shell (`myshell`) alongside a suite of simplified command-line utilities. Each utility is compiled as a standalone executable and is invoked by the main shell program using multi-processing system calls.

## Contributors
* **Nilay Choudhary (24JE0665)** - Implemented `custom_cp` and `custom_mv`.
* **Patel Het Alpeshbhai (24JE0670)** - Implemented `custom_wc`.
* **Paghdal Jeet Prakashkumar (24JE0667)** - Implemented `custom_echo`.
* **Nidhi Mithiya (24JE0664)** - Implemented `custom_ls` and `custom_cat`.
* **Sanjit Ram (24JE0666)** - Implemented `custom_rm`.

---

## Build and Execution Instructions

**To compile all binaries:**
```bash
make clean
make
```

**To compile and immediately launch the shell:**
```bash
make run
```

Once inside the shell (`myshell>`), you can type commands like:
```bash
custom_cp source.txt dest.txt
```
or type `exit` to terminate the shell.

---

## Core Architecture: The Custom Shell (`myshell`)

The shell operates in a continuous loop, parsing user input into arguments. When a recognized custom command is entered, the shell uses:

1. `fork()` → creates child process  
2. `execv()` → runs utility binary  
3. `waitpid()` → parent waits for child  

---

## Utilities Implemented

### 1. File Copying: `custom_cp`
**Implemented by:** Nilay Choudhary (24JE0665)

- **Usage:** `custom_cp <source> <destination>`
- **Description:** Copies file content
- **System Calls:** `open()`, `read()`, `write()`, `close()`
- **Details:** Uses 4096-byte buffer, overwrites destination

---

### 2. File Moving/Renaming: `custom_mv`
**Implemented by:** Nilay Choudhary (24JE0665)

- **Usage:** `custom_mv <source> <destination>`
- **Description:** Moves or renames file
- **System Calls:** `rename()`
- **Details:** Overwrites destination if exists

---

### 3. File Concatenation: `custom_cat`
**Implemented by:** Nidhi Mithiya (24JE0664)

- **Usage:** `custom_cat <file1> [file2...]`
- **Description:** Prints file contents
- **System Calls:** `open()`, `read()`, `write()`, `close()`
- **Details:** Supports multiple files, buffered reading

---

### 4. Directory Listing: `custom_ls`
**Implemented by:** Nidhi Mithiya (24JE0664)

- **Usage:** `custom_ls [directory]`
- **Description:** Lists directory contents
- **System Calls:** `opendir()`, `readdir()`, `closedir()`
- **Details:** Filters hidden files

---

### 5. Word Count: `custom_wc`
**Implemented by:** Patel Het Alpeshbhai (24JE0670)

- **Usage:** `custom_wc <filename>`
- **Description:** Counts lines, words, characters
- **System Calls:** `open()`, `read()`, `close()`
- **Details:** Buffered reading and counting logic

---

### 6. Echo: `custom_echo`
**Implemented by:** Paghdal Jeet Prakashkumar (24JE0667)

- **Usage:** `custom_echo [OPTION]... [STRING]...`
- **Description:** Prints strings
- **System Calls:** `fputs()`, `putchar()`
- **Details:** Supports `-n`, `-e`, `-E`

---

### 7. File Removal: `custom_rm`
**Implemented by:** Sanjit Ram (24JE0666)

- **Usage:** `custom_rm [-r] <file|directory> [file|directory ...]`
- **Description:** Removes files and directories from the filesystem
- **System Calls:** `stat()`, `unlink()`, `rmdir()`, `opendir()`, `readdir()`, `closedir()`
- **Details:**  
  - Supports multiple file and directory arguments  
  - Uses `stat()` to determine whether a path is a file or directory  
  - Regular files are removed using `unlink()`  
  - If the target is a directory without `-r`, an error is displayed  
  - With `-r`, directories are deleted recursively:
    - Opens directory using `opendir()`  
    - Iterates entries with `readdir()`  
    - Skips `.` and `..`  
    - Recursively deletes subdirectories  
    - Removes files using `unlink()`  
    - Finally removes the empty directory using `rmdir()`  
  - Uses `perror()` and error messages for failure cases  

---

## Proof of Execution
![Custom commands test](screenshot/custom_commands.png)
