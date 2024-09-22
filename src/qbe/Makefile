all: main.ssa
	qbe $^ -o out.s
	cc out.s -o out
	./out
