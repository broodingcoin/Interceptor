#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>


unsigned long **sys_call_table;

asmlinkage long (*ref_sys_cs3013_syscall1)(void);
asmlinkage long (*get_user_id)(void);
asmlinkage long (*ref_sys_open)(const char __user *filename, int flags, umode_t mode);
asmlinkage long (*ref_sys_close)(unsigned int fd);
asmlinkage long (*ref_sys_read)(int fd, void* buf, size_t count);


asmlinkage long new_sys_cs3013_syscall1(void) {
  printk(KERN_INFO "\"'Hello world?!' More like 'Goodbye, world!' EXTERMINATE!\" -- Dalek");
  return 0;
}

asmlinkage long new_sys_open(const char __user *filename, int flags, umode_t mode) {
  uid_t uid = get_user_id();
  if(uid >= 1000) {
    printk(KERN_INFO "User %d is opening file: %s\n", uid, filename);
  }
  return ref_sys_open(filename, flags, mode);
}

asmlinkage long new_sys_close(unsigned int fd) {
  uid_t uid = get_user_id();
  if(uid >= 1000) {
    printk(KERN_INFO "User %d is closing file descriptor: %d\n", uid, fd);
  }
  return ref_sys_close(fd);
}

asmlinkage long new_sys_read(int fd, void *buf, size_t count) {
  uid_t uid = get_user_id();
  char* target = "VIRUS";
  long ret = ref_sys_read(fd, buf, count);
  
  if(uid >= 1000){
    if(strstr(buf, target) == NULL){
      printk(KERN_INFO "User %d read from file descriptor: %d\n", uid, fd);
    }
    if(strstr(buf, target) != NULL){
      printk(KERN_INFO "User %d read from file descriptor: %d, but it contained malicious code!\n", uid, fd);
    }
  }
  return ret;
}

static unsigned long **find_sys_call_table(void) {
  unsigned long int offset = PAGE_OFFSET;
  unsigned long **sct;
  
  while (offset < ULLONG_MAX) {
    sct = (unsigned long **)offset;

    if (sct[__NR_close] == (unsigned long *) sys_close) {
      printk(KERN_INFO "Interceptor: Found syscall table at address: 0x%02lX",
	     (unsigned long) sct);
      return sct;
    }
    
    offset += sizeof(void *);
  }
  
  return NULL;
}

static void disable_page_protection(void) {
  write_cr0 (read_cr0 () & (~ 0x10000));
}

static void enable_page_protection(void) {
  write_cr0 (read_cr0 () | 0x10000);
}

static int __init interceptor_start(void) {
  /* Find the system call table */
  if(!(sys_call_table = find_sys_call_table())) {
    /* Well, that didn't work. 
       Cancel the module loading step. */
    return -1;
  }
  
  /* Store a copy of all the existing functions */
  ref_sys_cs3013_syscall1 = (void *)sys_call_table[__NR_cs3013_syscall1];
  get_user_id = (void *)sys_call_table[__NR_getuid];
  ref_sys_open = (void *)sys_call_table[__NR_open];
  ref_sys_close = (void *)sys_call_table[__NR_close];
  ref_sys_read = (void *)sys_call_table[__NR_read];

  /* Replace the existing system calls */
  disable_page_protection();

  sys_call_table[__NR_cs3013_syscall1] = (unsigned long *)new_sys_cs3013_syscall1;
  sys_call_table[__NR_open] = (unsigned long *)new_sys_open;  
  sys_call_table[__NR_close] = (unsigned long *)new_sys_close;
  sys_call_table[__NR_read] = (unsigned long *)new_sys_read;  

  enable_page_protection();
  
  /* And indicate the load was successful */
  printk(KERN_INFO "Loaded interceptor!");

  return 0;
}

static void __exit interceptor_end(void) {
  /* If we don't know what the syscall table is, don't bother. */
  if(!sys_call_table)
    return;
  
  /* Revert all system calls to what they were before we began. */
  disable_page_protection();
  sys_call_table[__NR_cs3013_syscall1] = (unsigned long *)ref_sys_cs3013_syscall1;
  sys_call_table[__NR_open] = (unsigned long *)ref_sys_open;  
  sys_call_table[__NR_close] = (unsigned long *)ref_sys_close;
  sys_call_table[__NR_read] = (unsigned long *)ref_sys_read;  
  enable_page_protection();

  printk(KERN_INFO "Unloaded interceptor!");
}

MODULE_LICENSE("GPL");
module_init(interceptor_start);
module_exit(interceptor_end);
