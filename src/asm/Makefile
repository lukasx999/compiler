all: main.asm
	nasm $^ -felf64 -o main.o
	ld main.o -o out
	./out
