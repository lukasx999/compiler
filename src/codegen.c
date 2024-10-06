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





// static void
// insert_string(vec_Vector *v, char *string, size_t index) {
//     vec_insert_before(v, index, &string);
// }


static void
append_string(vec_Vector *v, char *string) {
    vec_push(v, &string);
}



static char*
alloc_string_literal(Sections *s, const char *string, char **out_len) {

    static int i = 0;
    const char *preset_text = "strlit_text_";
    const char *preset_len = "strlit_len_";

    char *text = malloc(BUFSIZE * sizeof(char));
    sprintf(text, "%s%d", preset_text, i);

    char *len = malloc(BUFSIZE * sizeof(char));
    sprintf(len, "%s%d", preset_len, i);

    char *buf = malloc(BUFSIZE * sizeof(char));
    sprintf(buf, "\t%s: db \"%s\", 10\n"
                 "\t%s: db $-%s\n",
            text, string, len, text);

    append_string(&s->data, buf);
    ++i;

    *out_len = len;
    return text;

}







void _rec_compile(AstNode *root, Sections *sections) {

    switch (root->type) {

        case TYPE_FUNCTION: {

            append_string(&sections->text, "\n;--- function ---\n");

            if (!strcmp(root->ast_function.identifier.value, "main"))
                append_string(&sections->text, "global main\n");


            char *buf = malloc(BUFSIZE * sizeof(char));
            sprintf(buf, "%s:\n", root->ast_function.identifier.value);
            append_string(&sections->text, buf);


            append_string(&sections->text,
                          "\tpush rbp\n"
                          "\tmov rbp, rsp\n"
                          );

            // etc...
            _rec_compile(root->ast_function.body, sections);

            append_string(&sections->text,
                          "\tpop rbp\n"
                          "\tret\n"
                          ";--- end ---\n"
                          );

        } break;


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

void compile(AstNode *root) {

    // create 3 arrays of target code for each section (text, bss, data, rodata, ...)
    // then merge them together

    Sections sections;
    vec_init(&sections.text, sizeof(char*), 5, 2);
    append_string(&sections.text, "section .text\n");
    vec_init(&sections.data, sizeof(char*), 5, 2);
    append_string(&sections.data, "section .data\n");

    _rec_compile(root, &sections);

    FILE *file = fopen(ASM_FILENAME, "w");
    vec_extend(&sections.text, &sections.data); // merge sections

    for (size_t i=0; i<sections.text.size; ++i)
        fprintf(file, "%s", *(char**)vec_get(&sections.text, i));

    fclose(file);


    #if 1
    int id = fork();
    if (!id) execlp("nasm", "nasm", ASM_FILENAME, "-felf64", "-o", OBJ_FILENAME, "-g", "-F", "dwarf", NULL);
    wait(NULL);

    id = fork();
    if (!id) execlp("cc", "cc", OBJ_FILENAME, "-no-pie", "-ggdb", "-O0", "-o", BIN_FILENAME, NULL);
    wait(NULL);
    #endif



}
