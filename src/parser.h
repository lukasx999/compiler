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



// this lets a node point to multiple nodes, not just 1 or 2
typedef struct {
    size_t capacity, size;
    AstNode **nodes;
} AstNodeList;



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

typedef struct {
    AstNode *identifier; // Token?
    AstNode *value;
} ExprAssign;

typedef struct {
    AstNode *callee;
    // Token rparen;
    AstNodeList arguments;
} ExprCall;


/* -- ----------- -- */


/* -- STATEMENTS -- */

/* ETC */

// stores an identifier and a datatype as a pair
// eg: let x'int;
// identifier: x
// type: int
typedef struct {
    Token identifier, type;
} IdTypePair;


// statement sequence { ... } => scope
typedef struct {
    AstNodeList statements; // (dynamic) array containing a sequence of statements
    bool root;
} Block;


/* -- STATEMENTS -- */

typedef struct {
    AstNode *idtypepair;
    AstNode *value; // NULL if not yet specified
    bool mutable;
} StmtVarDeclaration;

typedef struct {
    Token identifier, returntype;
    AstNodeList parameters; // nodes == NULL if there are no parameters
    AstNode *body; // NULL if no body == declaration
} StmtFunction;

typedef struct {
    AstNode *expression;
    // Token keyword; // TODO: this
} StmtReturn;

typedef struct {
    AstNode *condition;
    AstNode *then_body;
    // AstNodeList elseif_bodies; // TODO: this
    AstNode *else_body; // NULL, if there is none
} StmtIf;

/* -- ---------- -- */


struct AstNode { // tagged union

    enum AstNode_type {
        // EXPRs
        TYPE_BINARYOP, TYPE_UNARYOP, TYPE_LITERAL, TYPE_GROUPING, TYPE_CALL,
        // STMTs
        TYPE_VARDECLARATION, TYPE_ASSIGN, TYPE_IF, TYPE_FUNCTION, TYPE_RETURN,
        // ETC
        TYPE_IDTYPEPAIR, TYPE_BLOCK,
    } type;

    union {
        ExprBinaryOp        ast_binaryop;
        ExprUnaryOp         ast_unaryop;
        ExprLiteral         ast_literal;
        ExprGrouping        ast_grouping;
        ExprAssign          ast_assign;
        ExprCall            ast_call;
        StmtVarDeclaration  ast_vardecl;
        StmtIf              ast_if;
        StmtFunction        ast_function;
        StmtReturn          ast_return;
        Block               ast_block;
        IdTypePair          ast_idtypepair;
    };

};

extern AstNode* ast_create_node (enum AstNode_type type, void *operation);
extern void ast_free_nodes (AstNode *root);

/* -- --- -- */




/* -- State of the Parser -- */

typedef struct {
    TokenList tokens;     // dynamic array of tokens
    char *source;         // source code: for errors
    size_t current;       // current token index
    const char *filename; // if empty -> repl
    uint32_t error_count; // if 0 -> no errors
} Parser;

extern Token    parser_get_current_token (Parser *p);
extern Token    parser_get_token_by_index(Parser *p, size_t index);
extern AstNode* parse(TokenList tokens, char *source, const char *filename); // returns NULL when there are no tokens to be parsed


/* -- Productions (rules) for the grammar -- */

extern AstNode
// expr
*rule_primary        (Parser *p),
*rule_unary          (Parser *p),
*rule_factor         (Parser *p),
*rule_term           (Parser *p),
*rule_comparison     (Parser *p),
*rule_equality       (Parser *p),
*rule_expression     (Parser *p),
// stmt
*rule_vardeclaration (Parser *p),
*rule_function       (Parser *p),
*rule_return         (Parser *p),
*rule_program        (Parser *p),
*rule_if             (Parser *p),
*rule_block          (Parser *p),
*rule_idtype_pair    (Parser *p),
*rule_assign         (Parser *p),
*rule_exprstatement  (Parser *p),
*rule_statement      (Parser *p),
*rule_statement      (Parser *p),
*rule_program        (Parser *p);

/* -- ----------------------------------- -- */

/* ERROR HANDLING */

extern void parser_throw_error (Parser *p, const char *message);
extern void parser_synchronize (Parser *p);
extern void parser_exit_errors (Parser *p);

/* -------------- */



#endif // _PARSER_H
