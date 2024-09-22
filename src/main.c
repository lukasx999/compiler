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

#include "lexer.h"
#include "repr.h"
#include "parser.h"
#include "typechecking.h"





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

        AstNode *root = parse(tokens, "");
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

    print_input(source);

    TokenList tokens = tokenize(source);
    print_tokens_columns(tokens);
    // print_tokens_stream(tokens);

    AstNode *root = parse(tokens, filename);
    print_ast(root);

}






int main(int argc, char **argv) {

    // REPL
    if (argc == 2 && !strcmp(argv[1], "repl"))
        repl();

    // RUN FROM FILE
    else if (argc == 3 && !strcmp(argv[1], "file")) {
        char *filename = argv[2];
        run_from_file(filename);
        exit(0);

    // DEV
    } else {

        // const char *input = "1+2*3";
        // const char *input = "10.";
        // const char *input = "()";
        // const char *input = "(3+4)*(3+4)";
        // const char *input = ")(";
        // const char *input = "1+2+3";
        // const char *input = "1==1==1";
        // const char *input = "(1+2+(1-2))*3;";
        // const char *input = "let foo_123 = (1.0+2)-'bar'*(-1);";
        // const char *input = "1+2; 3+4;";
        // const char *input = "letx = 5; let x = 5;";
        // const char *input = "let x = 5+(-1); let y = 3; let z = \"foo\"; let foobarbaz; let x;";
        // const char *input = "let foo'str = 5;";
        // const char *input = "x+1;";
        // const char *input = "let foo'int = 45;";
        // const char *input = "let foo'str = \"hello, world\";";
        // const char *input = "let foo'int = 4+5*(1/5+(1+(1+2))); foo = 45;";
        // const char *input = "{ let foo'int = (5) + (-5); } {{}}";
        const char *input = "defun main() -> int {}";

        // TODO: const char *input = "1);";

        print_input(input);

        TokenList tokens = tokenize(input);
        print_tokens_columns(tokens);
        // print_tokens_stream(tokens);

        AstNode *root = parse(tokens, "");
        print_ast(root);

        check_types(root);

        // Datatype result = evaluate(root);
        // printf("=== EVAL: ===\ndouble: %lf\nbool: %d\nstring: %s\n", result.d_int, result.d_bool, result.d_string);

        // cleanup
        // ast_free_nodes(root);
        // tokenlist_free(&tokens);

    }

    return EXIT_SUCCESS;

}
