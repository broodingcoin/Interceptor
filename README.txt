To run our code first "make", then use "sudo insmod interceptor.ko", to run the function, this is phase one.
after running phase one, use "sudo rmmod interceptor" to remove everything, then use "sudo insmod procAncestry.ko"
then you can run testPA.c to test the procAncestry function.
our function outputs everything to syslog properly.