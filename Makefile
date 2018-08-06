CFLAGS=-g -Wall -fPIC -I/usr/local/include
LIBS=-L/usr/local/lib

all: libHashTableO1

test: testHashTableO1
	./testHashTableO1 test-names-1m.txt

testHashTableO1: Makefile hashTableO1.h testHashTableO1.c
	gcc ${CFLAGS} -o testHashTableO1 testHashTableO1.c ${LIBS} -lHashTableO1

libHashTableO1: Makefile hashTableO1.o hashTableO1.h
	gcc -shared -o libHashTableO1.so.1.0 hashTableO1.o ${LIBS}

hashTableO1.o: Makefile hashTableO1.h hashTableO1.c
	gcc ${CFLAGS} -c hashTableO1.c -o hashTableO1.o

install:
	cp libHashTableO1.so.1.0 /usr/local/lib
	ln -sf /usr/local/lib/libHashTableO1.so.1.0 /usr/local/lib/libHashTableO1.so.1
	ln -sf /usr/local/lib/libHashTableO1.so.1.0 /usr/local/lib/libHashTableO1.so
	ldconfig /usr/local/lib
	cp hashTableO1.h /usr/local/include/hashTableO1.h

clean:
	rm *.o; rm *.so*; rm core*; rm testHashTableO1
