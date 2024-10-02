
ptr: ptr.asm
	nasm $^ -felf64 -o ptr.o -g -F dwarf
	cc ptr.o -no-pie -ggdb -O0
	./a.out

all: mod1.asm mod2.asm
	# gdb: info registers => i r
	# i r <register>
	nasm mod1.asm -felf64 -o mod1.o -g -F dwarf
	nasm mod2.asm -felf64 -o mod2.o -g -F dwarf
	cc mod1.o mod2.o -no-pie -ggdb -O0
	./a.out
