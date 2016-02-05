all: part1.o

part1.o:
	gcc part1.c -o part1.out

part1: all
	./part1.out

clean:
	rm part1.out