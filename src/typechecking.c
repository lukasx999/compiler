#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

#include "lexer.h"
#include "parser.h"


static void _rec_check_types(AstNode *root) {

    switch (root->type) {

        case TYPE_LITERAL: {
        } break;

        case TYPE_PROGRAMROOT: {
            size_t size = root->ast_programroot.statements.size;

            for (size_t i=0; i<size; ++i) {
                _rec_check_types(root->ast_programroot.statements.nodes[i]);
            }

        } break;

        default: { assert(!"invalid type"); } break;
    }

}


void check_types(AstNode *root) {

    _rec_check_types(root);

}
