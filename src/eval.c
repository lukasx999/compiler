#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "parser.h"
#include "lexer.h"
#include "eval.h"




/* -- EVALUATING THE AST -- */







Datatype eval_grouping(AstNode *n) {
    return evaluate(n->ast_grouping.node);
}


Datatype eval_literal(AstNode *n) {

    Token token = n->ast_literal.operator;
    Datatype new;

    switch (token.type) {

        case TOK_LITERAL_FLOAT:
        case TOK_LITERAL_INTEGER: {
            new.kind = KIND_INTEGER;
            new.d_int = strtof(token.value, NULL);
        } break;

        case TOK_LITERAL_TRUE: {
            new.kind = KIND_BOOLEAN;
            new.d_bool = true;
        } break;

        case TOK_LITERAL_FALSE: {
            new.kind = KIND_BOOLEAN;
            new.d_bool = false;
        } break;

        case TOK_LITERAL_STRING: {
            new.kind = KIND_STRING;
            strncpy(new.d_string, token.value, BUFSIZE);
        } break;

        case TOK_LITERAL_NIL: {
            new.kind = KIND_NIL;
            new.d_nil = true;
        } break;

        default: {
            assert(!"wrong token type for literal");
        } break;

    }

    return new;

}



Datatype eval_unaryop(AstNode *n) {

    Datatype new;
    Datatype node = evaluate(n->ast_unaryop.node);

    switch (n->ast_unaryop.operator.type) {

        case TOK_BIN_UNR_MINUS: {
            new.d_int = -node.d_int; // TODO: check datatypes
        } break;

        case TOK_UNR_BANG: {
            new.d_bool = !node.d_bool;
        } break;

        case TOK_BIN_UNR_SLASH: {
            new.d_int = 1 / node.d_int;
        } break;

        default: {
            assert(!"wrong token type for literal");
        } break;

    }

    return new;

}




Datatype eval_binaryop(AstNode *n) {

    Datatype new;
    Datatype left = evaluate(n->ast_binaryop.left_node);
    Datatype right = evaluate(n->ast_binaryop.right_node);
    // FIX: problem: mixing boolean calculations with float ones, eg: 1==2==3 -> true==3

    switch (n->ast_binaryop.operator.type) {

        // Arithmetic operations
        case TOK_BIN_PLUS: {
            new.kind = KIND_INTEGER;
            new.d_int = left.d_int + right.d_int;
        } break;

        case TOK_BIN_UNR_MINUS: {
            new.kind = KIND_INTEGER;
            new.d_int = left.d_int - right.d_int;
        } break;

        case TOK_BIN_UNR_SLASH: {
            new.kind = KIND_INTEGER;
            new.d_int = left.d_int / right.d_int;
        } break;

        case TOK_BIN_ASTERISK: {
            new.kind = KIND_INTEGER;
            new.d_int = left.d_int * right.d_int;
        } break;

        // Boolean operations
        case TOK_BIN_EQUAL: {
            new.kind = KIND_BOOLEAN,
            new.d_bool = left.d_int == right.d_int;
        } break;

        case TOK_BIN_EQUAL_NOT: {
            new.kind = KIND_BOOLEAN,
            new.d_bool = left.d_int != right.d_int;
        } break;



        default: {
            assert(!"wrong token type for literal");
        } break;

    }

    return new;

}








Datatype evaluate(AstNode *root) {

    if (root == NULL) return (Datatype){0};
    Datatype new;

    switch (root->type) {
        case TYPE_BINARYOP: {
            new = eval_binaryop(root);
        } break;

        case TYPE_UNARYOP: {
            new = eval_unaryop(root);
        } break;

        case TYPE_LITERAL: {
            new = eval_literal(root);
        } break;

        case TYPE_GROUPING: {
            new = eval_grouping(root);
        } break;


        default: {
            assert(!"non-existant type");
        } break;

    }

    return new;

}



/* -- ------------------ -- */
