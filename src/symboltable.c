
#include "symboltable.h"
#include "sym_hashtable.h"


static void
traverse_ast(AstNode *root, vec_Vector *tables, HashTable *current) {

    switch (root->type) {

        case TYPE_FUNCTION: {

            HashTable new;
            ht_init(&new, 50, 25, 2);

            traverse_ast(root->ast_function.body, tables, &new);

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
                HashTable new;
                ht_init(&new, 50, 25, 2);

                vec_Vector *v = &root->ast_block.statements;
                for (size_t i = 0; i < v->size; ++i) {
                    traverse_ast((AstNode*)vec_get(v, i), tables, &new);
                }

                vec_push(tables, &new);
                break;

            }

            vec_Vector *v = &root->ast_block.statements;
            for (size_t i = 0; i < v->size; ++i) {
                traverse_ast((AstNode*)vec_get(v, i), tables, current);
            }


        } break;

        case TYPE_VARDECLARATION: {

            #if 0
            SymbolTableEntry entry = {
                .identifier  = root->ast_vardecl.idtypepair->ast_idtypepair.identifier,
                .rbp_offset  = 0,
                .scope_level = 0,
                .datatype    = root->ast_vardecl.idtypepair->ast_idtypepair.type
            };

            KVPair data = { .value = entry, .key = (ht_Key_t){ .identifier = entry.identifier.value, .scope_level = entry.scope_level } };
            ht_insert(current, data);

            #endif
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

    traverse_ast(root, &tables, NULL);


    // puts("::");
    // for (size_t i = 0; i < tables.size; ++i) {
    //     ht_print((HashTable*)vec_get(&tables, i));
    //     puts("::");
    // }


}
