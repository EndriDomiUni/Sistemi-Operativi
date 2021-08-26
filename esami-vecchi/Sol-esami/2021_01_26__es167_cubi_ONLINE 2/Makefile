CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	cubi.exe

cubi.exe: cubi.o DBGpthread.o
	gcc ${CFLAGS} -o cubi.exe cubi.o DBGpthread.o ${LIBRARIES}

cubi.o: cubi.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} cubi.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f cubi.o DBGpthread.o cubi.exe


