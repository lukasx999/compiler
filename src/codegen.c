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
    vec_push(v, &string);
}

static void
insert_string(vec_Vector *v, const char *string, size_t index) {
    vec_insert_before(v, index, &string);
}



void _rec_compile(AstNode *root, vec_Vector *lines) {

    switch (root->type) {

        case TYPE_FUNCTION: {

            append_string(lines, "\n;--- function ---\n");

            if (!strcmp(root->ast_function.identifier.value, "main"))
                append_string(lines, "global main\n");

            char buf[BUFSIZE];
            sprintf(buf, "%s:\n", root->ast_function.identifier.value);
            append_string(lines, (const char*)&buf);

            append_string(lines, "\tpush rbp\n");
            append_string(lines, "\tmov rbp, rsp\n");

            // etc...
            _rec_compile(root->ast_function.body, lines);

            append_string(lines, "\tpop rbp\n");
            append_string(lines, "\tret\n");
            append_string(lines, ";--- end ---\n");

        } break;


        case TYPE_ECHO: {

            append_string(lines, "\n;--- echo ---\n");
            append_string(lines, "\tmov rax, 1\n"  );
            append_string(lines, "\tmov rdi, 1\n"  );
            append_string(lines, "\tmov rsi, msg\n");
            append_string(lines, "\tmov rdx, len\n");
            append_string(lines, "\tsyscall\n"     );
            append_string(lines, ";--- end ---\n");

            insert_string(lines, "\tlen: db $-msg\n", 0);
            insert_string(lines, "\tmsg: db \"Hello, World\"\n", 0);
            insert_string(lines, "section .data\n", 0);

        } break;

        // case TYPE_RETURN: {
        //     append_string(lines, "\tmov rax, retval\n");
        // } break;

        case TYPE_BLOCK: {
            size_t size = root->ast_block.statements.size;
            for (size_t i=0; i<size; ++i)
                _rec_compile(vec_get(&root->ast_block.statements, i), lines);
        } break;



        default: break;
        // default: assert(!"invalid type"); break;
    }

}

void compile(AstNode *root) {

    vec_Vector lines;
    vec_init(&lines, sizeof(char*), 5, 2);
    append_string(&lines, "section .text");


    _rec_compile(root, &lines);


    FILE *file = fopen(ASM_FILENAME, "w");

    for (size_t i=0; i<lines.size; ++i)
        fprintf(file, "%s", *(char**)vec_get(&lines, i));

    fclose(file);


    #if 0
    int id = fork();
    if (!id) execlp("nasm", "nasm", ASM_FILENAME, "-felf64", "-o", OBJ_FILENAME, "-g", "-F", "dwarf", NULL);
    wait(NULL);

    id = fork();
    if (!id) execlp("cc", "cc", OBJ_FILENAME, "-no-pie", "-ggdb", "-O0", "-o", BIN_FILENAME, NULL);
    wait(NULL);
    #endif



}
