all: part1.o part2.o part3.o

part1.o:
	gcc pa1_part1.c -o pa1_part1

part2.o:
	gcc pa1_part2.c -o pa1_part2

part3.o:
	gcc pa1_part3.c -o pa1_part3

part1: all
	./pa1_part1

part2: all
	./pa1_part2

part3: all
	./pa1_part3

clean:
	rm pa1_part1
	rm pa1_part2
	rm pa1_part3