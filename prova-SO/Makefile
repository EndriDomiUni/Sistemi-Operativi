CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	prova.exe

prova.exe: prova.o DBGpthread.o
	gcc ${CFLAGS} -o prova.exe prova.o DBGpthread.o ${LIBRARIES}

prova.o: prova.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} prova.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f prova.o DBGpthread.o prova.exe