obj-m += cool.o
ccflags-y += -Wno-unused-result -mavx2 -mpreferred-stack-boundary=4

all: cool.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

insert:
	sudo insmod cool.ko

remove:
	sudo rmmod cool

reinsert: remove insert