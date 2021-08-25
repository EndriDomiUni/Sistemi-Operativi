CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	guado.exe

guado.exe: guado.o DBGpthread.o
	gcc ${CFLAGS} -o guado.exe guado.o DBGpthread.o ${LIBRARIES}

guado.o: guado.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} guado.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f guado.o DBGpthread.o guado.exe


