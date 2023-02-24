#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "userprog/process.h"
#include "threads/pte.h"
#include "userprog/pagedir.h"
#include "filesys/filesys.h"

static void syscall_handler(struct intr_frame*);

// System calls
void sys_halt(void);
void sys_exit(int status);
pid_t sys_exec(const char *cmd_line);
int sys_wait(pid_t pid);
bool sys_create(const char *file, unsigned initial_size);
bool sys_remove(const char *file);
int sys_open(const char *file);
int sys_file_size(int fd);
int sys_read(int fd, char *buffer, unsigned size);
int sys_write(int fd, const char *buffer, unsigned size);
void sys_close(int fd);
int sys_practice(int i);
void sys_seek(int fd, unsigned position);
unsigned sys_tell(int fd);

// User pointer validation
void check_user_stack_addresses(uint32_t* uaddr, size_t num_bytes);


void syscall_init(void) { 
  intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall"); 
  lock_init(&fileop_lock);
}

static void syscall_handler(struct intr_frame* f UNUSED) {
  uint32_t* args = ((uint32_t*)f->esp);
  /*
   * The following print statement, if uncommented, will print out the syscall
   * number whenever a process enters a system call. You might find it useful
   * when debugging. It will cause tests to fail, however, so you should not
   * include it in your final submission.
   */

  // printf("System call number: %d\n", args[0]);

  check_user_stack_addresses(args, 4);
  if (args[0] < 4)
  {
    switch(args[0])
    {
      case SYS_HALT:
        //TODO
        break;
      case SYS_EXIT:
        //TODO
        f->eax = args[1];
        sys_exit(args[1]);
        break;
      case SYS_EXEC:
        //TODO
        break;
      case SYS_WAIT:
        //TODO
        break;
    }
  }
  else
  {
    lock_acquire(&fileop_lock);
    switch(args[0])
    {
      case SYS_CREATE:
        //TODO
        check_user_stack_addresses(args + 1, 8);
        check_user_stack_addresses((uint32_t* ) args[1], 1);
        f->eax = sys_create((const char*) args[1], args[2]);
        break;
      case SYS_REMOVE:
        //TODO
        check_user_stack_addresses(args + 1, 4);
        check_user_stack_addresses((uint32_t*) args[1], 1);
        f->eax = sys_remove((const char*) args[1]);
        break; 
      case SYS_OPEN:
        //TODO
        break;
      case SYS_FILESIZE:
        //TODO
        break;
      case SYS_READ:
        //TODO
        break;
      case SYS_WRITE:
        //TODO
        f->eax = sys_write(args[1], (const char*)args[2], args[3]);
        break;
      case SYS_SEEK:
        //TODO
        break;
      case SYS_TELL:
        //TODO
        break;
      case SYS_CLOSE:
        //TODO
        break;
      case SYS_PRACTICE:
        //TODO
        f->eax = sys_practice(args[1]);
        break; 
    }
    lock_release(&fileop_lock);
  }
}

//   switch(args[0])
//   {
//     case SYS_CLOSE:
//       // TODO
//       break;
//     case SYS_CREATE:
//       // TODO
//       lock_aquire(&fileop_lock);
//       f->eax = sys_create(args[1], args[2]);
//       lock_release(&fileop_lock);
//       break;
//     case SYS_EXEC:
//       // TODO
//       break;
//     case SYS_EXIT:
//       //TODO
//       f->eax = args[1];
//       sys_exit(args[1]);
//       break;
//     case SYS_FILESIZE:
//       // TODO
//       break;
//     case SYS_HALT:
//       // TODO
//       break;
//     case SYS_OPEN:
//       // TODO
//       break;
//     case SYS_PRACTICE:
//       // TODO
//       f->eax = sys_practice(args[1]);
//       break;
//     case SYS_READ:
//       // TODO
//       break;
//     case SYS_REMOVE:
//       // TODO
//       break;
//     case SYS_SEEK:
//       // TODO
//       break;
//     case SYS_TELL:
//       // TODO
//       break;
//     case SYS_WAIT:
//       // TODO
//       break;
//     case SYS_WRITE:
//       //TODO
//       f->eax = sys_write(args[1], (const char*)args[2], args[3]);
//       break;
//   }
// }



void sys_halt() {
  // TODO
}

void sys_exit(int status) {
  // TODO
  struct thread* t = thread_current();
  t->pcb->status = status;
  process_exit();
}

pid_t sys_exec(const char *cmd_line) {
  // TODO
}

int sys_wait(pid_t pid) {
  // TODO
}

bool sys_create(const char *file, unsigned initial_size) {
  // TODO
  return filesys_create(file, initial_size);
}

bool sys_remove(const char *file) {
  // TODO
  return filesys_remove(file);
}

int sys_open(const char *file) {
  // TODO
}

int sys_file_size(int fd) {
  // TODO
}
int sys_read(int fd, char *buffer, unsigned size) {
  // TODO
}

int sys_write(int fd, const char *buffer, unsigned size) {
  // TODO
  if (fd == 1)
  {
    putbuf(buffer, size);

    // Not sure what to return when writing to the console
    return size;
  }
  return 0;
}

void sys_seek(int fd, unsigned position) {
  // TODO
}
unsigned sys_tell(int fd) {
  // TODO
}

void sys_close(int fd) {
  // TODO
}

int sys_practice(int i) {
  return ++i;
}


void check_user_stack_addresses(uint32_t* uaddr, size_t num_bytes) {
  // TODO
  if (uaddr == NULL) {
    sys_exit(-1);
  }
  // Cast to char* so ++ operator increments ptr by one byte
  char* uaddr_cpy = (char*) uaddr;
  for (size_t i = 0; i < num_bytes; i++) {
    if (!is_user_vaddr((uint32_t* )uaddr_cpy)) {
      sys_exit(-1);
    }
    else if (pagedir_get_page(thread_current()->pcb->pagedir, (const void*) uaddr_cpy) == NULL) {
      sys_exit(-1);
    }
    uaddr_cpy++;
  }

}