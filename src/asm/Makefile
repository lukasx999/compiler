all: fork.asm
	# gdb: info registers => i r
	# i r <register>
	nasm $^ -felf64 -o main.o -g -F dwarf
	cc main.o -no-pie -ggdb -O0
	./a.out
