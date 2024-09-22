all: main.asm
	nasm $^ -felf64 -o main.o
	cc main.o -no-pie
	./a.out
