#include <linux/sched.h>
#include <linux/list.h>
#include "ancestors.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>

#define __NR_cs3013_syscall2 378

long testCall2 (unsigned short *target_pid, struct ancestry *response) {
  return (long) syscall(__NR_cs3013_syscall2, target_pid, response);
}

int main() {
  struct ancestry *family = malloc(sizeof(ancestry*));
  char pid[10], *p, *e;
  long v;

  printf("Enter the target pid!");
 
  while(fgets(pid, sizeof(pid), stdin)) {
    p = pid;
    for(p = pid ;p = e) {
      v = strtol(p, &e, 10);
      if(p == e) {
	break;
      }
    }
  }

  testCall2(pid, family);

  for(int i = 0; family->ancestors.size(); i++) {
    printf("%d is a parent of %d\n", family->ancestors, target_pid);
  }

  for(int i = 0; family->siblings.size(); i++) {
    printf("%d is a sibling of %d\n", family->sibling, target_pid);
  }

  for(int i = 0; family->children.size(); i++) {
    printf("%d is a child of %d\n", family->children, target_pid);
  }

  return 0;
  /*int pi = fork();
  if(pid < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(1);
  }
  else if(pid == 0) {
    char *myargs[4];
    myargs[0] = strdup("syscall2");   
    myargs[1] = strdup(target_pid);
    myargs[2] = strdup(family);
    myargs[3] = strdup(NULL);
    execvp(myargs[0], myargs);
  }
  else{
    int wc = wait(NULL);
  }*/
  
}
