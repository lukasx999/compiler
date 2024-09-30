.PHONY: clean

CC=gcc
PYTHON=python3
EXTRAFLAGS=-Wno-unused-variable
CFLAGS=-Wall -Wextra -std=c17 -pedantic -ggdb -O0 $(EXTRAFLAGS)
BIN=compiler
TEST=test



all: main.c lexer.c parser.c repr.c extra/vector.c codegen.c semantic_analysis.c
	$(PYTHON) getgrammar.py
	$(CC) $(CFLAGS) -o $(BIN) $^ -I./extra

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

clean:
	rm *.o $(BIN) $(TEST)

test: test.o lexer.o parser.o eval.o repr.o
	$(CC) $(CFLAGS) $^ -o $(TEST)
