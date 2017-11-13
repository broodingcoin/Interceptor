obj-m := interceptor.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: test
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f *.o
	rm -f test

test: test.o
	gcc -g test.o -o test

test.o: test.c
	gcc -g -c test.c
