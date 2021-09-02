CFLAGSCONSTRERROR=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L
CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -ggdb

LIBRARIES=-lpthread
DBGPTHREADFUNCTION_SOURCE_DIR=./

all: 	 catapulte.exe

catapulte.exe:  catapulte.o DBGpthread.o 
	gcc ${CFLAGS} -o catapulte.exe catapulte.o DBGpthread.o ${LIBRARIES}

catapulte.o: catapulte.c ${DBGPTHREADFUNCTION_SOURCE_DIR}DBGpthread.h ${DBGPTHREADFUNCTION_SOURCE_DIR}printerror.h
	gcc -c ${CFLAGS} -I${DBGPTHREADFUNCTION_SOURCE_DIR} catapulte.c

DBGpthread.o: ${DBGPTHREADFUNCTION_SOURCE_DIR}DBGpthread.c ${DBGPTHREADFUNCTION_SOURCE_DIR}printerror.h
	gcc ${CFLAGSCONSTRERROR} -c ${DBGPTHREADFUNCTION_SOURCE_DIR}DBGpthread.c -I${DBGPTHREADFUNCTION_SOURCE_DIR}


.PHONY:	clean

clean:
	-rm -f DBGpthread.o  catapulte.exe catapulte.o 


