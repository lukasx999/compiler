#pragma once
#ifndef _PARSER_H
#define _PARSER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "lexer.h"


/* Recursive Descent Parser */






typedef struct AstNode AstNode; // forward reference

/* -- EXPRESSIONS -- */

typedef struct {
    AstNode *left_node, *right_node;
    Token operator;
} ExprBinaryOp;

typedef struct {
    AstNode *node;
    Token operator;
} ExprUnaryOp;

typedef struct {
    Token operator;
} ExprLiteral;

typedef struct {
    AstNode *node;
} ExprGrouping;

/* -- ----------- -- */


/* -- STATEMENTS -- */

/* ETC */

// this lets a node point to multiple nodes, not just 1 or 2
typedef struct {
    size_t capacity, size;
    AstNode **nodes;
} AstNodeList;


// stores an identifier and a datatype as a pair
// eg: let x'int;
// identifier: x
// type: int
typedef struct {
    Token identifier, type;
} IdTypePair;



/* --------- */


// statement sequence
typedef struct {
    AstNodeList statements; // (dynamic) array containing statements
} ProgramRoot;


// statement sequence { ... } => scope
typedef struct {
    AstNodeList statements;
} Block;



/* -- STATEMENTS -- */

typedef struct {
    AstNode *idtypepair;
    AstNode *value; // NULL if not yet specified
} StmtVarDeclaration;

typedef struct {
    Token identifier;
    AstNode *value;
} StmtAssign;




typedef struct {
    Token identifier, returntype;
    AstNodeList arguments; // nodes == NULL if there are no arguments
    AstNode *body;
} StmtFunction;

typedef struct {
    AstNode *expression;
} StmtReturn;

typedef struct {
    AstNode *condition;
    AstNode *if_body;
    AstNode *else_body; // NULL, if there is none
} StmtIf;

/* -- ---------- -- */


struct AstNode { // tagged union

    enum AstNode_type {
        // EXPRs
        TYPE_BINARYOP, TYPE_UNARYOP, TYPE_LITERAL, TYPE_GROUPING,
        // STMTs
        TYPE_VARDECLARATION, TYPE_ASSIGN, TYPE_IF, TYPE_FUNCTION, TYPE_RETURN,
        // ETC
        TYPE_PROGRAMROOT, TYPE_IDTYPEPAIR, TYPE_BLOCK,
    } type;

    union {
        ExprBinaryOp        ast_binaryop;
        ExprUnaryOp         ast_unaryop;
        ExprLiteral         ast_literal;
        ExprGrouping        ast_grouping;
        StmtVarDeclaration  ast_vardecl;
        StmtIf              ast_if;
        StmtFunction        ast_function;
        StmtReturn          ast_return;
        StmtAssign          ast_assign;
        ProgramRoot         ast_programroot;
        Block               ast_block;
        IdTypePair          ast_idtypepair;
    };

};

extern AstNode* ast_create_node (enum AstNode_type type, void *operation);
extern void ast_free_nodes (AstNode *root);

/* -- --- -- */




/* -- State of the Parser -- */


typedef struct {

    TokenList tokens;
    size_t current;
    const char *filename; // if empty -> repl
    // TODO: add error state
    // while parsing, add up errors, then dump them all at once and exit(1)
    bool error;

} Parser;




extern Token get_current_token(Parser *p);
extern AstNode* parse(TokenList tokens, const char *filename); // returns NULL when there are no tokens to be parsed


/* -- ------------------- -- */



/* -- Productions (rules) for the grammar -- */

extern AstNode
// expr
*rule_primary     (Parser *p),
*rule_unary       (Parser *p),
*rule_factor      (Parser *p),
*rule_term        (Parser *p),
*rule_comparison  (Parser *p),
*rule_equality    (Parser *p),
*rule_expression  (Parser *p),
// stmt
*rule_vardeclaration     (Parser *p),
*rule_exprstatement    (Parser *p),
*rule_statement   (Parser *p),
*rule_statement (Parser *p),
*rule_program     (Parser *p);

/* -- ----------------------------------- -- */


/* ERROR HANDLING */

enum ErrorTypes {
    ERROR_INVALIDTOKEN,
    ERROR_EMPTYGROUPING,
    ERROR_UNMATCHEDPAREN,
    ERROR_INCORRECTTYPE,
    ERROR_EXPECTEDEXPR,
    ERROR_UNEXPECTEDTOKEN,
    ERROR_EXPECTEDSEMICOLON,
};

extern void parser_throw_error(Parser *p, enum ErrorTypes type);

/* -------------- */



#endif // _PARSER_H
