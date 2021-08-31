CFLAGSCONSTRERROR=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L
CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE 

LIBRARIES=-lpthread
DBGPTHREADFUNCTION_SOURCE_DIR=./

all: 	 turisti.exe

turisti.exe:  turisti.o DBGpthread.o 
	gcc ${CFLAGS} -o turisti.exe turisti.o DBGpthread.o ${LIBRARIES}

turisti.o: turisti.c ${DBGPTHREADFUNCTION_SOURCE_DIR}DBGpthread.h ${DBGPTHREADFUNCTION_SOURCE_DIR}printerror.h
	gcc -c ${CFLAGS} -I${DBGPTHREADFUNCTION_SOURCE_DIR} turisti.c

DBGpthread.o: ${DBGPTHREADFUNCTION_SOURCE_DIR}DBGpthread.c ${DBGPTHREADFUNCTION_SOURCE_DIR}printerror.h
	gcc ${CFLAGSCONSTRERROR} -c ${DBGPTHREADFUNCTION_SOURCE_DIR}DBGpthread.c -I${DBGPTHREADFUNCTION_SOURCE_DIR}


.PHONY:	clean

clean:
	-rm -f DBGpthread.o  turisti.exe turisti.o 


