#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>

// These values MUST match the unistd_32.h modifications:
#define __NR_cs3013_syscall1 377

long testCall1 ( void) {
return (long) syscall(__NR_cs3013_syscall1);
}

int main(){
printf("\tcs3013_syscall1: %ld\n", testCall1());
	FILE *test;
	int fd;
	char buf[128];

	fd = open("testfile.txt", O_RDONLY);
	read(fd, buf, 128); //force a read
	close(fd);
	fd = open("testbadfile.txt", O_RDONLY);
	read(fd, buf, 128); //force a read from the malicious code-containing file
	close(fd);

return 0;
}
