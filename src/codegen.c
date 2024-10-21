#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <assert.h>
#include <stddef.h>

#include "codegen.h"




static void
append_string(vec_Vector *v, const char *string) {
    for (size_t i = 0; i < strlen(string); ++i)
        vec_push(v, (void*)&string[i]);
}

#define INTERPOLATE(vector, fmt, ...) { \
    char buf[BUFSIZE_BIG] = { 0 }; \
    sprintf(buf, fmt, __VA_ARGS__); \
    append_string(vector, buf); \
} assert(true)



void _rec_compile(AstNode *root, Sections *sections) {

    switch (root->type) {

        case TYPE_BINARYOP: {
            enum TokenType type = root->ast_binaryop.operator.type;

            switch (type) {
                case TOK_BIN_PLUS: {
                    append_string(&sections->text, "\tadd rax, ");
                } break;
                case TOK_BIN_UNR_MINUS: {
                    append_string(&sections->text, "\tsub rax, ");
                } break;
                case TOK_BIN_ASTERISK: {
                    append_string(&sections->text, "\timul rax, ");
                } break;
                case TOK_BIN_UNR_SLASH: { // TODO: this
                    append_string(&sections->text, "\tdiv rax, ");
                } break;
                default: break;
            }

            _rec_compile(root->ast_binaryop.left_node,  sections);
            _rec_compile(root->ast_binaryop.right_node, sections);

        } break;


        case TYPE_LITERAL: {
            char *value         = root->ast_literal.operator.value;
            enum TokenType type = root->ast_literal.operator.type;

            INTERPOLATE(&sections->text, " %s\n", value);

        } break;


        case TYPE_FUNCTION: {
            append_string(&sections->text, "\n;--- function ---\n");

            // TODO: add `global` keyword
            if (!strcmp(root->ast_function.identifier.value, "main"))
                append_string(&sections->text, "global main\n");

            // char buf[BUFSIZE_BIG] = { 0 };
            // sprintf(buf, "%s:\n", root->ast_function.identifier.value);
            // append_string(&sections->text, buf);
            INTERPOLATE(&sections->text, "%s:\n", root->ast_function.identifier.value);

            append_string(&sections->text,
                          "\tpush rbp\n"
                          "\tmov rbp, rsp\n");

            // etc...
            _rec_compile(root->ast_function.body, sections);

            append_string(&sections->text,
                          "\tpop rbp\n"
                          "\tret\n"
                          ";--- end ---\n");

        } break;


        #if 0
        case TYPE_ECHO: {

            assert(root->ast_echo.value->type == TYPE_LITERAL);
            assert(root->ast_echo.value->ast_literal.operator.type == TOK_LITERAL_STRING);

            char *message = root->ast_echo.value->ast_literal.operator.value;

            char *out_len = malloc(BUFSIZE * sizeof(char));
            char *text = alloc_string_literal(sections, message, &out_len);

            char *buf = malloc(BUFSIZE * sizeof(char));
            sprintf(buf,
                          "\n;--- echo ---\n"
                          "\tmov rax, 1\n"
                          "\tmov rdi, 1\n"
                          "\tmov rsi, %s\n"
                          "\tmov rdx, %s\n"
                          "\tsyscall\n"
                          ";--- end ---\n", text, out_len
            );

            append_string(&sections->text, buf);

        } break;
        #endif

        // case TYPE_RETURN: {
        //     append_string(&sections->text, "\tmov rax, retval\n");
        // } break;

        case TYPE_BLOCK: {
            size_t size = root->ast_block.statements.size;
            for (size_t i=0; i<size; ++i)
                _rec_compile(vec_get(&root->ast_block.statements, i), sections);
        } break;



        default: break;
        // default: assert(!"invalid type"); break;
    }

}



static void
actually_compile(void) {

    int id = fork();
    if (!id) execlp("nasm", "nasm",
                    ASM_FILENAME,
                    "-felf64",
                    "-o", OBJ_FILENAME,
                    "-g", "-F", "dwarf",
                    NULL);
    wait(NULL);

    id = fork();
    if (!id) execlp("cc", "cc",
                    OBJ_FILENAME,
                    "-no-pie",
                    "-ggdb", "-O0",
                    "-o", BIN_FILENAME,
                    NULL);
    wait(NULL);

}

void compile(AstNode *root) {

    // create 3 arrays of target code for each section (text, bss, data, rodata, ...)
    // then merge them together

    Sections sections;

    vec_init(&sections.text, sizeof(char), 5, 2);
    append_string(&sections.text, "section .text\n");

    vec_init(&sections.data, sizeof(char), 5, 2);
    append_string(&sections.data, "\nsection .data\n");

    _rec_compile(root, &sections);

    vec_extend(&sections.text, &sections.data); // merge sections

    // write vector to file
    FILE *file = fopen(ASM_FILENAME, "w");
    for (size_t i=0; i<sections.text.size; ++i)
        fprintf(file, "%c", *(char*)vec_get(&sections.text, i));
    fclose(file);

    // actually_compile();

}
