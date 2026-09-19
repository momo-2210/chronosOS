obj-m += chronos.o
chronos-objs := src/chronos_core.o src/chronos_syscall.o src/chronos_sched.o src/chronos_blockio.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
