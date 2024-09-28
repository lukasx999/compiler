#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

#include "codegen.h"


void _rec_compile(AstNode *root, FILE *file) {

    switch (root->type) {

        case TYPE_FUNCTION: {
            fprintf(file, "\n;--- function ---\n");

            if (!strcmp(root->ast_function.identifier.value, "main"))
                fprintf(file, "global main\n");

            fprintf(file, "%s:\n", root->ast_function.identifier.value);
            fprintf(file, "\tpush rbp\n");
            fprintf(file, "\tmov rbp, rsp\n");

            // etc...
            _rec_compile(root->ast_function.body, file);

            fprintf(file, "\tpop rbp\n");
            fprintf(file, "\tret\n");
            fprintf(file, ";--- end ---\n");

        } break;



        case TYPE_BLOCK: {
            size_t size = root->ast_block.statements.size;
            for (size_t i=0; i<size; ++i)
                _rec_compile(root->ast_block.statements.nodes[i], file);
        } break;



        default: break;
        // default: assert(!"invalid type"); break;
    }

}

void compile(AstNode *root) {
    FILE *file = fopen("out.s", "w");
    _rec_compile(root, file);
    fclose(file);
}
