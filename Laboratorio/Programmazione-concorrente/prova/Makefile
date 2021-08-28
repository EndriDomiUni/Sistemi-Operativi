CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	fachiriV2.exe

fachiriV2.exe: fachiriV2.o DBGpthread.o
	gcc ${CFLAGS} -o fachiriV2.exe fachiriV2.o DBGpthread.o ${LIBRARIES}

fachiriV2.o: fachiriV2.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} fachiriV2.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f fachiriV2.o DBGpthread.o fachiriV2.exe


