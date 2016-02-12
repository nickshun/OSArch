all: part1.o part2.o

part1.o:
	gcc part1.c -o part1.out

part2.o:
	gcc part2.c -o part2.out

part1: all
	./part1.out

part2: all
	./part2.out

clean:
	rm part1.out
	rm part2.out