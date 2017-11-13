#include <linux/sched.h>
#include <linux/list.h>


struct ancestry{
  pid_t ancestors[10];
  pid_t siblings[100];
  pid_t children[100];
};

long procAncestry(unsigned short *target_pid, struct ancestry *response) {
  unsigned short buf;
  struct pid *pid_struct;
  struct task_struct *mytask;
  struct task_struct *mytask2;

  if(copy_from_user(&buf, target_pid, sizeof(target_pid))) {
    return -EFAULT;
  }
  
  pid_struct = find_get_pid(target_pid);
  mytask = pid_task(pid_struct, PIDTYPE_PID);
  
  mytask2 = mytask;

  while(mytask2->parent != NULL) {
    ancestry->ancestors = mytask2->parent;
    mytask2 = mytask2-parent;
   
  }
  
  
}
