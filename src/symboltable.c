#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "symboltable.h"
#include "sym_hashtable.h"



static KVPair
get_pair_idtypepair(IdTypePair *pair) {


    SymbolTableEntry entry = {
        .identifier  = pair->identifier,
        .rbp_offset  = 0,
        .scope_level = 0,
        .datatype    = pair->type
    };

    ht_Key_t key = { .scope_level = entry.scope_level };
    strcpy(key.identifier, entry.identifier.value);

    KVPair data = { .value = entry, .key = key };
    return data;

}


static void
traverse_ast(AstNode *root, vec_Vector *tables, HashTable *current) {

    assert(!((current == NULL) && !(root->type == TYPE_BLOCK))); // sanity check - if current is NULL, node must be a block (we dont want to segfault)

    switch (root->type) {

        case TYPE_FUNCTION: {

            HashTable new;
            ht_init(&new, 50, 25, 2);

            traverse_ast(root->ast_function.body, tables, &new);


            vec_Vector *params = &root->ast_function.parameters;
            for (size_t i = 0; i < params->size; ++i) {
                IdTypePair pair = *(IdTypePair*) vec_get(params, i);
                KVPair kv = get_pair_idtypepair(&pair);
                ht_insert(&new, kv);
            }

            vec_push(tables, &new);

        } break;

        case TYPE_BLOCK: {

            vec_Vector *v = &root->ast_block.statements;

            if (root->ast_block.root) {
                HashTable new;
                ht_init(&new, 50, 25, 2);

                for (size_t i = 0; i < v->size; ++i)
                    traverse_ast((AstNode*)vec_get(v, i), tables, &new);

                vec_push(tables, &new);
                break;
            }

            for (size_t i = 0; i < v->size; ++i)
                traverse_ast((AstNode*)vec_get(v, i), tables, current);


        } break;

        case TYPE_VARDECLARATION: {
            KVPair kv = get_pair_idtypepair(&root->ast_vardecl.idtypepair);
            ht_insert(current, kv);
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


    #if 0
    HashTable t;
    ht_init(&t, sizeof(HashTable), 5, 2);

    SymbolTableEntry entry = { .identifier = (Token){.value = "foo"} };
    ht_Key_t key = { .identifier = "foo", .scope_level = 0 };
    KVPair kv = { .value = entry, .key = key };
    ht_insert(&t, kv);

    entry = (SymbolTableEntry){ .identifier = (Token){.value = "bar"} };
    key = (ht_Key_t){ .identifier = "bar", .scope_level = 0 };
    kv = (KVPair){ .value = entry, .key = key };
    ht_insert(&t, kv);

    ht_print(&t);
    #endif



    puts("===");
    for (size_t i = 0; i < tables.size; ++i) {
        ht_print((HashTable*)vec_get(&tables, i));
        puts("===");
    }


}
