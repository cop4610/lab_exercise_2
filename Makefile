CFLAGS = -Wall -ggdb -lpthread

all: part1.c
	${CC} ${CFLAGS} -o part11 $^
	${CC} ${CFLAGS} -DPTHREAD_SYNC -o part12 $^

clean:
	-rm -f *~ *.o *.dep part1 part12
