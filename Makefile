all: part1.o part2.o

part1.o:
	gcc pa1_part1.c -o pa1_part1

part2.o:
	gcc pa1_part2.c -o pa1_part2

part1: all
	./pa1_part1

part2: all
	./pa1_part2

clean:
	rm pa1_part1
	rm pa1_part2