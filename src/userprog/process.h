#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"
#include <stdint.h>

// At most 8MB can be allocated to the stack
// These defines will be used in Project 2: Multithreading
#define MAX_STACK_PAGES (1 << 11)
#define MAX_THREADS 127

/* PIDs and TIDs are the same type. PID should be
   the TID of the main thread of the process */
typedef tid_t pid_t;

/* Thread functions (Project 2: Multithreading) */
typedef void (*pthread_fun)(void*);
typedef void (*stub_fun)(pthread_fun, void*);

struct process;
struct shared_data_struct;

// Global lock for all file operations
// struct lock fileop_lock;

/* The process control block for a given process. Since
   there can be multiple threads per process, we need a separate
   PCB from the TCB. All TCBs in a process will have a pointer
   to the PCB, and the PCB will have a pointer to the main thread
   of the process, which is `special`. */

/* PCB
  children = list of shared data structs for each of its children.
  max_fd = highest index in FDT.  New opened files will get fd = max_fd + 1
  executable = file pointer to executable file.  Used to prevent other processes from writing to file after it has been loaded.
*/
struct process {
  /* Owned by process.c. */
  uint32_t* pagedir;          /* Page directory. */
  char process_name[16];      /* Name of the main thread */
  struct thread* main_thread; /* Pointer to main thread */
  struct list children;       //List of all children pcb's
  struct list fdt;
  int max_fd;
  struct file* executable;
  //   struct list threads;
  //   struct semaphore wait_status; //Semaphore that will be upped after child let's parent run
  //   struct lock ref_cnt_lock;  //Lock so parent and child do not access ref_cnt at same time
  struct shared_data_struct* shared_data;
  pid_t pid;
  struct lock child_list_lock;
  struct dir* cwd;
};

struct shared_data_struct {
  char* fn_copy;
  struct process* pcb;
  int shared_data_status;
  int load_status;
  struct semaphore shared_data_sema;
  struct lock shared_data_lock;
  struct list_elem elem;
  pid_t pid;
  int ref_count;
  bool parent_waiting;
};

struct fdt_entry {
  struct list_elem elem;
  struct file* file;
  struct dir* dir;
  int fd;
  //TODO: create is_dir var
  bool is_dir;
};

void userprog_init(void);

pid_t process_execute(const char* file_name);
int process_wait(pid_t);
void process_exit(void);
void process_activate(void);

bool is_main_thread(struct thread*, struct process*);
pid_t get_pid(struct process*);

tid_t pthread_execute(stub_fun, pthread_fun, void*);
tid_t pthread_join(tid_t);
void pthread_exit(void);
void pthread_exit_main(void);

#endif /* userprog/process.h */
