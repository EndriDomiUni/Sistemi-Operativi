CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	fachiri.exe

fachiri.exe: fachiri.o DBGpthread.o
	gcc ${CFLAGS} -o fachiri.exe fachiri.o DBGpthread.o ${LIBRARIES}

fachiri.o: fachiri.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} fachiri.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f fachiri.o DBGpthread.o fachiri.exe


