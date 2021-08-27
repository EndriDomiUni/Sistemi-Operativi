CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	banche.exe

banche.exe: banche.o DBGpthread.o
	gcc ${CFLAGS} -o banche.exe banche.o DBGpthread.o ${LIBRARIES}

banche.o: banche.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} banche.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f banche.o DBGpthread.o banche.exe


