.PHONY: clean test

CC         = gcc
PYTHON     = python3
EXTRAFLAGS = -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
CFLAGS     = -Wall -Wextra -std=c17 -pedantic -ggdb -O0 $(EXTRAFLAGS) -I./extra
BIN        = compiler
TEST       = test

all: $(BIN) grammar.txt test

grammar.txt: getgrammar.py parser.c
	$(PYTHON) $<

$(BIN): main.o lexer.o parser.o repr.o extra/vector.o codegen.o symboltable.o err.o sym_hashtable.o
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "CC *.o"

%.o: %.c %.h Makefile
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "CC" $<

test:
	@$(PYTHON) test.py

clean:
	rm *.o $(BIN)
