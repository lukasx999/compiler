
#include "symboltable.h"
#include "sym_hashtable.h"


static void
traverse_ast(AstNode *root, vec_Vector *tables) {

    switch (root->type) {

        case TYPE_FUNCTION: {

            HashTable new;
            ht_init(&new, 50, 25, 2);



            traverse_ast(root->ast_function.body, tables);


            vec_push(tables, &new);


            #if 0
            size_t size = root->ast_function.parameters.size;
            for (size_t i = 0; i < size; ++i) {
                AstNode *node = (AstNode*) vec_get(&root->ast_function.parameters, i);
                traverse_ast(node);
            }
            #endif



        } break;

        case TYPE_BLOCK: {

            if (root->ast_block.root) {
                // HashTable new;
                // ht_init(&new, 50, 25, 2);
                // traverse_ast(root->ast_function.body, tables);
                // vec_push(tables, &new);
            }

        } break;

        case TYPE_VARDECLARATION: {
        } break;

        // function parameters
        case TYPE_IDTYPEPAIR: {
        } break;



        default: break;
    }


}


void
construct_symboltable(AstNode *root) {

    vec_Vector tables;
    vec_init(&tables, sizeof(HashTable), 5, 2);

    traverse_ast(root, &tables);

}
