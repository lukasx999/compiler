
strings: strings.asm
	nasm $^ -felf64 -o strings.o -g -F dwarf
	cc strings.o -no-pie -ggdb -O0 -o out
	./out

all: mod1.asm mod2.asm
	# gdb: info registers => i r
	# i r <register>
	nasm mod1.asm -felf64 -o mod1.o -g -F dwarf
	nasm mod2.asm -felf64 -o mod2.o -g -F dwarf
	cc mod1.o mod2.o -no-pie -ggdb -O0
	./a.out
