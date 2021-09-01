CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	incrocio.exe

incrocio.exe: incrocio.o DBGpthread.o
	gcc ${CFLAGS} -o incrocio.exe incrocio.o DBGpthread.o ${LIBRARIES}

incrocio.o: incrocio.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} incrocio.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f incrocio.o DBGpthread.o incrocio.exe


