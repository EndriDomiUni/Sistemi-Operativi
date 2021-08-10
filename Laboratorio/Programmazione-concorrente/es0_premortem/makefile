# MakeFile

CFLAGS=-ansi -Wpedantic -Wall -D_THREAD_SAFE -D_REENTRANT -D_POSIX_C_SOURCE=20200112L
LFLAGS=-lpthread

all: es0_premortem.exe

es0_premortem.exe: es0_premortem.o
	gcc ${CFLAGS} -0 es0_premortem.exe es0_premortem.o ${LFlAGS}

es0_premortem.o: es0_premortem.c
	gcc -c ${CFLAGS} es0_premortem.c

.PHONY: clean

clean:
	rm -f es0_premortem.exe es0_premortem.o
