#pragma once
#ifndef _PARSER_H
#define _PARSER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "lexer.h"


/* Recursive Descent Parser */

typedef struct AstNode AstNode;
typedef vec_Vector AstNodeList;

/* -- EXPRESSIONS -- */

typedef struct {
    AstNode *left_node,  // [expr]
            *right_node; // [expr]
    Token operator;
} ExprBinaryOp;

typedef struct {
    AstNode *node; // [expr]
    Token operator;
} ExprUnaryOp;

typedef struct {
    Token operator;
} ExprLiteral;

typedef struct {
    AstNode *node; // [expr]
} ExprGrouping;

typedef struct {
    AstNode *identifier; // [literal-id] TODO: Token?
    AstNode *value;      // [expr]
} ExprAssign;

typedef struct {
    AstNode *callee; // [expr]
    // Token rparen;
    AstNodeList arguments;
} ExprCall;


/* -- ----------- -- */

/* -- STATEMENTS -- */

/* UTIL */

// stores an identifier and a datatype as a pair
typedef struct {
    Token identifier, type;
} IdTypePair;

// statement sequence = scope
typedef struct {
    AstNodeList statements;
    bool root;
} Block;

/* ---- */

/* -- STATEMENTS -- */

typedef struct {
    AstNode *idtypepair; // [idtypepair]
    AstNode *value;      // [expr] | NULL = no initialization
    bool mutable;
    Token keyword;
} StmtVarDeclaration;

typedef struct {
    Token identifier, returntype, keyword;
    AstNodeList parameters;
    AstNode *body; // [block] | NULL = declaration
} StmtFunction;

typedef struct {
    AstNode *value; // [expr]
    Token keyword;
} StmtReturn;

typedef struct {
    AstNode *value; // [expr]
    Token keyword;
} StmtEcho;

typedef struct {
    AstNode *condition; // [expr]
    AstNode *then_body; // [block]
    AstNode *else_body; // [block]
    Token keyword;
} StmtIf;

/* -- ---------- -- */


struct AstNode {

    enum AstNode_type {
        // EXPRs
        TYPE_BINARYOP, TYPE_UNARYOP, TYPE_LITERAL, TYPE_GROUPING, TYPE_CALL,
        // STMTs
        TYPE_VARDECLARATION, TYPE_ASSIGN, TYPE_IF, TYPE_FUNCTION, TYPE_RETURN, TYPE_ECHO,
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
        StmtEcho            ast_echo;
        Block               ast_block;
        IdTypePair          ast_idtypepair;
    };

};

extern AstNode* ast_create_node (enum AstNode_type type, void *operation);
extern void ast_free_nodes (AstNode *root);

/* -- --- -- */




/* -- State of the Parser -- */

typedef struct {
    TokenList  tokens;      // dynamic array of tokens
    size_t     current;     // current token index
    uint32_t   error_count; // if 0 -> no errors

    // these are mainly just for errors
    char       *source;     // source code: for errors
    const char *filename;   // if empty -> repl
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

enum ErrorType {
    ERROR_CUSTOM,
    ERROR_EXPECTED_SEMICOLON,
    ERROR_EXPECTED_RETURNTYPE,
    ERROR_EXPECTED_ARROW,
    ERROR_TYPE_SPECIFIER_MISSING,
};

extern void parser_throw_error (Parser *p, enum ErrorType type, const char *message);
extern void parser_synchronize (Parser *p);
extern void parser_exit_errors (Parser *p);

/* -------------- */



#endif // _PARSER_H
