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



static Table* // return NULL if no new symbol table is created
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

            TableColumn col;
            col.identifier = root->ast_vardecl.idtypepair->ast_idtypepair.identifier.value;
            col.type = get_type_from_token(root->ast_vardecl.idtypepair->ast_idtypepair.type);

            if (root->ast_vardecl.value == NULL)
                col.value = NULL;

            vec_push(&table->rows, &col);
            return NULL;

        } break;

        case TYPE_FUNCTION: {

            TableColumn col;
            col.type = DATATYPE_FUNCTION;
            col.identifier = root->ast_function.identifier.value;

            if (root->ast_function.body == NULL)
                col.not_initialized = true;

            vec_push(&table->rows, &col);

            return traverse(root->ast_function.body, table);
        } break;



        case TYPE_LITERAL: {
        } break;




        default: return NULL; break;
        // default: assert(!"unknown astnode type"); break;

    }

    // return new;

}




Table*
check_semantics(AstNode *root) {

    Table *table = traverse(root, NULL);
    return table;

}
