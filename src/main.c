#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "lexer.h"
#include "repr.h"
#include "parser.h"
#include "codegen.h"
#include "symboltable.h"





#define REPL_PROMPT "> "

void repl(void) {

    system("clear");

    while (1) {

        char input[BUFSIZE];
        printf(REPL_PROMPT);

        char *ret = fgets(input, BUFSIZE, stdin);
        input[strcspn(input, "\n")] = 0;

        // <C-d>
        if (ret == NULL) {
            puts("");
            exit(0);
        }

        system("clear");

        print_input(input);

        TokenList tokens = tokenize(input);
        print_tokens_stream(tokens);

        AstNode *root = parse(tokens, input, "");
        print_ast(root);

    }

}




char* read_file(char *filename) {

    FILE *f = fopen(filename, "r");
    char c;

    struct stat s;
    stat(filename, &s);
    size_t size = s.st_size;

    char *chars = calloc(size, sizeof(char) + 1);
    size_t chars_index = 0;

    while ((c = fgetc(f)) != EOF) {
        chars[chars_index] = c;
        ++chars_index;
    }

    return chars;

}


void run_from_file(char *filename) {

    char *source = read_file(filename);
    // print_input(source);

    TokenList tokens = tokenize(source);
    // print_tokens_stream(tokens);
    // print_tokens_columns(tokens);

    AstNode *root = parse(tokens, source, filename);
    print_ast(root);

    Table *table = construct_symboltable(root);
    print_symboltable(table);
    compile(root);

}


int main(int argc, char **argv) {

    // REPL
    if (argc == 2 && !strcmp(argv[1], "repl"))
        repl();

    // RUN FROM FILE
    else if (argc == 2) {
        char *filename = argv[1];
        run_from_file(filename);
        exit(0);

    // DEV
    } else {

        const char *input = "defun main() -> int {}";
        print_input(input);

        TokenList tokens = tokenize(input);
        print_tokens_columns(tokens);

        AstNode *root = parse(tokens, (char*)input, "");
        print_ast(root);

    }

    return EXIT_SUCCESS;

}
