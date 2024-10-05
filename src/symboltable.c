#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "symboltable.h"



static enum Datatype
get_type_from_token(Token tok) {
    switch (tok.type) {

        case TOK_KEYWORD_DATATYPE_INT:  return DATATYPE_INTEGER; break;
        case TOK_KEYWORD_DATATYPE_STR:  return DATATYPE_STRING;  break;
        case TOK_KEYWORD_DATATYPE_BOOL: return DATATYPE_BOOL;    break;
        case TOK_KEYWORD_DATATYPE_VOID: return DATATYPE_VOID;    break;

        default: assert(!"invalid type"); break;
    }
}




Table*
table_create(Table *parent) {

    Table *new = malloc(sizeof(Table));

    new->parent = parent;

    vec_Vector children;
    vec_init(&children, sizeof(Table), 5, 2);
    new->children = children;

    vec_Vector rows;
    vec_init(&rows, sizeof(TableColumn), 5, 2);
    new->rows = rows;

    return new;

}


// return NULL if no new symbol table is created
// pass the current table to a blocks children
// - if child is block
//  - child returns a new table (!= NULL)
//  - link the new child table with to parent one
// - if child is something else (eg: vardecl)
//  - return NULL (parent doesnt add table)
//  - append now row to table

static Table*
traverse(AstNode *root, Table *table) {

    enum AstNode_type type = root->type;

    switch (type) {
        case TYPE_BLOCK: {

            Table *new = table_create(table);

            size_t size = root->ast_block.statements.size;
            for (size_t i = 0; i < size; ++i) {
                Table *t = traverse(vec_get(&root->ast_block.statements, i), new);
                if (t != NULL)
                    vec_push(&new->children, t);
            }

            return new;

        } break;

        case TYPE_VARDECLARATION: {

            // needs to be pointer, else string will go out of function scope
            IdTypePair *pair = &root->ast_vardecl.idtypepair->ast_idtypepair;

            TableColumn col;
            col.identifier = pair->identifier.value;
            col.type       = get_type_from_token(pair->type);
            col.address    = 0;

            if (root->ast_vardecl.value == NULL)
                col.not_initialized = true;

            vec_push(&table->rows, &col);
            return NULL;

        } break;

        case TYPE_FUNCTION: {

            TableColumn col;
            col.type = DATATYPE_FUNCTION;
            col.identifier = root->ast_function.identifier.value;
            col.address = 0;

            if (root->ast_function.body == NULL)
                col.not_initialized = true;

            vec_push(&table->rows, &col);

            Table *t = traverse(root->ast_function.body, table);
            t->stack_frame = true;
            return t;

        } break;



        case TYPE_LITERAL: {
        } break;




        default: return NULL; break;
        // default: assert(!"unknown astnode type"); break;

    }

    // return new;

}




static void
populate_addresses(Table *root) {

    size_t size_count = 0;
    size_t vector_size = root->rows.size;

    // if (root->stack_frame) {

        for (size_t i=0; i<vector_size; ++i) {
            TableColumn *child = vec_get(&root->rows, i);
            size_t size = datatype_size[child->type];
            size_count += size;
            child->address = size_count;
        }

    // }


    vector_size = root->children.size;
    for (size_t i=0; i<vector_size; ++i) {
        Table *child = (Table*) vec_get(&root->children, i);
        populate_addresses(child);
    }

}



Table*
check_semantics(AstNode *root) {

    Table *table = traverse(root, NULL);
    populate_addresses(table);
    return table;

}
