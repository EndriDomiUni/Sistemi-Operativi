CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	keppall2.exe

keppall2.exe: keppall2.o DBGpthread.o
	gcc ${CFLAGS} -o keppall2.exe keppall2.o DBGpthread.o ${LIBRARIES}

keppall2.o: keppall2.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} keppall2.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f keppall2.o DBGpthread.o keppall2.exe


