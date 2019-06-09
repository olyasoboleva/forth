all: main

main: main.o
	ld -o main main.o

main.o: main.asm
	nasm -f elf64 -g -F dwarf main.asm -o main.o

clean:
	rm -f main *.o
