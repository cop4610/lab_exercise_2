CFLAGS = -Wall -ggdb -lpthread

all: part1 part2

part1: part1.c
	${CC} ${CFLAGS} -o part11 $^
	${CC} ${CFLAGS} -DPTHREAD_SYNC -o part12 $^
	${CC} ${CFLAGS} -DSYSCALL_SYNC -o part131 $^
	${CC} ${CFLAGS} -DSYSCALL_SYNC -DPTHREAD_SYNC -o part132 $^

part2: part2.c
	${CC} ${CFLAGS} -o part2 $^

clean:
	-rm -f *~ *.o *.dep part1 part12 part2
