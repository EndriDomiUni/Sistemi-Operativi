CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	rw2.exe

rw2.exe: rw2.o DBGpthread.o
	gcc ${CFLAGS} -o rw2.exe rw2.o DBGpthread.o ${LIBRARIES}

rw2.o: rw2.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} rw2.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f rw2.o DBGpthread.o rw2.exe


