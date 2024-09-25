#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "semantic_analysis.h"








static enum Datatype
get_type_from_token(Token tok) {
}



static void
traverse(AstNode *root) {

    enum AstNode_type type = root->type;

    // STNode *new = stnode_create();
    // TableList list;
    // stnodeslist_init(&list);


    switch (type) {


        case TYPE_BLOCK: {

            // stnodeslist_append(&list, new);


            size_t size = root->ast_block.statements.size;

            // for (size_t i=0; i<size; ++i) {
            //     root->ast_block.statements.nodes[i];
            // }




        } break;



        case TYPE_LITERAL: {

            // STEntry entry;

        } break;




        default: assert(!"unknown astnode type"); break;

    }

    // return new;

}




void
check_semantics(AstNode *root) {

    // traverse(root);


}
