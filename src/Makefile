.PHONY: clean

CC=gcc
CPP=g++
PYTHON=python
EXTRAFLAGS=-Wno-unused-variable
CFLAGS=-Wall -Wextra -std=c17 -pedantic -ggdb -O0 $(EXTRAFLAGS)
CPPFLAGS=-Wall -Wextra -std=c++17 -ggdb -O0
BIN=compiler
TEST=test



all: main.c lexer.c parser.c repr.c typechecking.c
	$(CPP) $(CPPFLAGS) -o pre preprocessor.cxx
	$(PYTHON) getgrammar.py # Generate grammar from comments
	$(CC) $(CFLAGS) -o $(BIN) $^

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

clean:
	rm *.o $(BIN) $(TEST)

test: test.o lexer.o parser.o eval.o repr.o
	$(CC) $(CFLAGS) $^ -o $(TEST)
