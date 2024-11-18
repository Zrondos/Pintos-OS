## User Features

1. Process Management: Users can use system calls to:
    - Create a process and run C Files or command line
    - Create child processes and manage their execution order using the `wait` system call
2. Thread Management: Users can use system calls to:
    - Create multiple threads per process
    - Control execution order using the `join` system call
    - Perform fine-grained synchronization and concurrency using locks and semaphores
3. File System Interaction: Users can use system calls to:
    - Create, remove, open, and close files
    - Read to and write to files
    - Create directory hierarchy with subdirectories
    - Resolve absolute and relative paths

## **Implementation Details**

1. Memory Design
    - Page tables are used to create a virtual to physical memory address space for each process
    - Each user process is given its own page directory with a virtual memory address space of 4 MB
    - Demand paging is performed upon page faults by allocating or loading pages in memory
2. Process Address Space
    - Each processes’ virtual memory is split into kernel and user memory
    - Each process has a virtual address space for its stack
    - The stack is split into kernel memory and user memory
    - Each process can have multiple threads, each with their own address space in kernel memory and user memory
    - For security, address validation is used to prevent users from accessing kernel memory, memory from other processes, and unmapped memory
3. 80x86 Architecture/ ISA
    - `x86` assembly interfaces with hardware to save and restore the CPU state of a process
    - `x86` paging mechanism, with a two-level page table system, translates virtual addresses to physical memory addresses.
    - All interrupts and system calls are managed safely using the `x86` Interrupt Descriptor Table
    - `x86` architecture uses a permissions register to prevent malicious users from accessing protected registers such as kernel permissions and page table pointers
4. Context Switches
    - Users can only access the kernel using system calls, interrupts, traps, and exceptions
    - Users pass control to the kernel using the `int` `x86` instruction
    - The kernel passes control to the user using the `iret` `x86` instruction
    - The kernel stores/restores that state of the process’s CPU registers in/from kernel memory
5. Thread Scheduling
    - The kernel schedules threads based on priority
    - Priority donation is used to prevent deadlocks
    - The kernel uses locks, semaphores, and condition variables for safe concurrency
    - Preemptive scheduling is used to prevent malicious processes from hogging CPU time
6. Security and Safety
    - Paging and pointer validation prevents a process from accessing kernel memory or the memory of another process
    - Safe handling of interrupts by saving/restoring CPU execution state and only allowing a single entry point to kernel mode
    - Preemptive thread scheduling prevents threads from hogging CPU time
    - Priority donation prevents deadlocks and priority inversion
    - Synchronization primitives like locks and semaphores ensure safe access to shared resources
    - Kernel panics, page faults, and assertions halt faulty or malicious processes
7. File System Design
    - A Unix inspired file system uses 512 B inodes to manage file metadata and data locations on disk
    - The OS initializes and formats an 8 MB disk with 512 B blocks
    - File system initialization creates a root directory and a bitmap to track disk space usage
    - The bitmap efficiently allocates new blocks for the creation of new files and directories, and the dynamic growing and shrinking of files
8. Extensible Filesystem
    - Users can dynamically grow files from 0 to 8 MB without needing contiguous blocks of memory or causing external fragmentation
    - Each file is represented as an inode, that contains direct, indirect, and doubly indirect blocks pointing to non-contiguous sectors/blocks of disk
9. Buffer Cache
    - A 32 KB buffer cache reduces disk I/O latency
    - Asynchronous read-ahead caching allows for concurrent performance of I/O and computation
    - Cache entries are evicted using LRU
    - Dirty evicted entries are written to disk ensuring data consistency
    - When a dirty entry is evicted, other dirty entries are written in a single disk operation
    - Periodic flushes write dirty entries to disk to prevent data loss during crashes
    - Synchronization prevents memory inconsistency by ensuring safe access to shared resources like disk