KDIR = /lib/modules/`uname -r`/build

all:
	make -C $(KDIR) M=`pwd`
	$(CC) randomNumberUserSpace.c -o randomNumber_UserSpace
clean:
	make -C $(KDIR) M=`pwd` clean
	rm randomNumber_UserSpace
