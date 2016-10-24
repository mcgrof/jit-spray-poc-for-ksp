rm=/bin/rm -f
CC=cc
DEFS=
INCLUDES=-I.
LIBS=

DEFINES= $(INCLUDES) $(DEFS)
CFLAGS= $(DEFINES) -g -Wall -Wextra -O2 -D_GNU_SOURCE


alameda: alameda.c
	$(CC) $(CFLAGS) -o alameda alameda.c $(LIBS)

helper: helper.c
	$(CC) $(CFLAGS) -c helper.c $(LIBS)

all: alameda helper

.PHONY: distclean clean preliminary
clean:
	$(rm) alameda helper *.o core *~

distclean: clean
	sudo rmmod jump

preliminary:
	@(cd ko && make)
	sudo insmod ko/jump.ko
	echo 1 | sudo tee /proc/sys/net/core/bpf_jit_enable >/dev/null
	echo 0 | sudo tee /proc/sys/kernel/kptr_restrict >/dev/null
