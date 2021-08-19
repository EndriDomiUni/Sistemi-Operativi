# MakeFile

CFLAGS=-ansi -Wpedantic -Wall -D_THREAD_SAFE -D_REENTRANT -D_POSIX_C_SOURCE=20200112L
LFLAGS=-lpthread

all: es0000_strutture.exe

es0000_strutture.exe: es0000_strutture.o
	gcc ${CFLAGS} -o es0000_strutture.exe es0000_strutture.o ${LFlAGS}

es0000_strutture.o: es0000_strutture.c
	gcc -c ${CFLAGS} es0000_strutture.c

.PHONY: clean

clean:
	rm -f es0000_strutture.exe es0000_strutture.o