#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <stdarg.h>

#include "lexer.h"
#include "parser.h"
#include "repr.h" // just for the colors

#define MATCH_SENTINEL (-1)



#define BINARYEXPR(callback, ...) {                            \
                                                               \
    AstNode *new = callback(p);                                \
                                                               \
    while (match_tokentypes(p, __VA_ARGS__, MATCH_SENTINEL)) { \
                                                               \
        Token operator = get_current_token(p);                 \
        ++p->current;                                          \
                                                               \
        AstNode *right = callback(p);                          \
                                                               \
        ExprBinaryOp op = { .left_node  = new,                 \
                            .right_node = right,               \
                            .operator   = operator };          \
        new = ast_create_node(TYPE_BINARYOP, &op);             \
                                                               \
    }                                                          \
                                                               \
    return new;                                                \
}





#define PRINT_ERROR() fprintf(stderr, "%s%sERROR: %s", COLOR_RED, COLOR_BOLD, COLOR_END);

void parser_throw_customerror(const char *message) {
    PRINT_ERROR();
    fprintf(stderr, "%s\n", message);
    exit(1);
}


void parser_throw_error(Parser *p, enum ErrorTypes type) {
    p->error = true;

    Token current = get_current_token(p);


    switch (type) {

        case ERROR_INVALIDTOKEN: {
            if (!strcmp(p->filename, "")) // repl, no filename
                fprintf(stderr, "(repl): ERROR: invalid token `%s`\n", current.value);
            else
                fprintf(stderr, "%s:%lu:%lu: ERROR: invalid token `%s`\n", p->filename, current.line_number, current.column, current.value);
        } break;

        case ERROR_EMPTYGROUPING: {
            PRINT_ERROR();
            fprintf(stderr, "empty groupings are not supported\n");
        } break;

        case ERROR_UNMATCHEDPAREN: {
            PRINT_ERROR();
            fprintf(stderr, "unmatched parenthesis\n");
        } break;

        case ERROR_EXPECTEDEXPR: {
            PRINT_ERROR();
            fprintf(stderr, "expected expression (got `eof`)\n");
        } break;

        case ERROR_UNEXPECTEDTOKEN: {
            PRINT_ERROR();
            fprintf(stderr, "unexpected token: `%s`\n", token_repr[current.type]);
        } break;

        case ERROR_EXPECTEDSEMICOLON: {
            PRINT_ERROR();
            fprintf(stderr, "expected semicolon after statement\n");
        } break;

        default:
            assert(!"invalid error type");
            break;
    }

    exit(1);

}






/* AST NODE LIST */

void astnodelist_init(AstNodeList *a) {

    a->capacity = 5;
    a->size = 0;
    a->nodes = malloc(a->capacity * sizeof(AstNode*));

}


void astnodelist_append(AstNodeList *a, AstNode **new) {

    if (a->size+1 == a->capacity) {
        a->capacity *= 2;
        a->nodes = realloc(a->nodes, a->capacity * sizeof(AstNode*));
    }

    a->nodes[a->size++] = *new;

}

/* ------------- */









// `operation` gets casted to corresponding type, according to `Node_type`
AstNode* ast_create_node(enum AstNode_type type, void *operation) {

    AstNode *new = malloc(sizeof(AstNode));

    new->type = type;

    switch (type) {

        case TYPE_BINARYOP:       new->ast_binaryop    = *(ExprBinaryOp*)        operation; break;
        case TYPE_UNARYOP:        new->ast_unaryop     = *(ExprUnaryOp*)         operation; break;
        case TYPE_LITERAL:        new->ast_literal     = *(ExprLiteral*)         operation; break;
        case TYPE_GROUPING:       new->ast_grouping    = *(ExprGrouping*)        operation; break;
        case TYPE_VARDECLARATION: new->ast_vardecl     = *(StmtVarDeclaration*)  operation; break;
        case TYPE_IF:             new->ast_if          = *(StmtIf*)              operation; break;
        case TYPE_FUNCTION:       new->ast_function    = *(StmtFunction*)        operation; break;
        case TYPE_RETURN:         new->ast_return      = *(StmtReturn*)          operation; break;
        case TYPE_ASSIGN:         new->ast_assign      = *(StmtAssign*)          operation; break;
        case TYPE_PROGRAMROOT:    new->ast_programroot = *(ProgramRoot*)         operation; break;
        case TYPE_BLOCK:          new->ast_block       = *(Block*)               operation; break;
        case TYPE_IDTYPEPAIR:     new->ast_idtypepair  = *(IdTypePair*)          operation; break;

        default: assert(!"non-existant type"); break;

    }

    return new;

}

// traverse the ast and free all nodes from the bottom up
// TODO: complete this
void ast_free_nodes(AstNode *root) {

    if (root == NULL) return;

    switch (root->type) {

        case TYPE_BINARYOP: {
            AstNode *left = root->ast_binaryop.left_node;
            AstNode *right = root->ast_binaryop.right_node;
            ast_free_nodes(left);
            ast_free_nodes(right);
        } break;

        case TYPE_UNARYOP: {
            AstNode *node = root->ast_unaryop.node;
            ast_free_nodes(node);
        } break;

        case TYPE_LITERAL:
            break;

        case TYPE_GROUPING: {
            AstNode *node = root->ast_grouping.node;
            ast_free_nodes(node);
        } break;


        default:
            assert(!"non-existant type");
            break;

    }

    free(root);

}






//--------------------------------------------------------------------------------


Token get_current_token(Parser *p) {
    return p->tokens.tokens[p->current];
}


// matches to current token in the parser against any of the supplied token types
// this is a pure convience function
bool match_tokentypes(Parser *p, ...) {

    enum TokenType input = get_current_token(p).type;

    va_list args;
    va_start(args, p);

    enum TokenType type;

    for (;;) {

        type = va_arg(args, int);

        if ((int) type == MATCH_SENTINEL)
            break;

        if (input == type) {
            return true;
            va_end(args);
        }

    }

    va_end(args);
    return false;

}








/* -- EXPRESSIONS -- */

AstNode* rule_primary(Parser *p) {
    // BNF: primary -> "true" | "false" | "nil" | NUMBER | STRING | "(" expression ")" | IDENTIFIER

    AstNode *new = NULL;

    if (match_tokentypes(p, TOK_LPAREN, MATCH_SENTINEL)) {

        p->current++;
        AstNode *group = rule_expression(p); // get the content inside of the group

        ExprGrouping operation = { .node = group };
        new = ast_create_node(TYPE_GROUPING, &operation);

        // check for matched paren
        // TODO: switch to match_tokentypes
        if (get_current_token(p).type != TOK_RPAREN)
            parser_throw_error(p, ERROR_UNMATCHEDPAREN);

        p->current++;

    } else if (match_tokentypes(p, TOK_INVALID, MATCH_SENTINEL)) {
        parser_throw_error(p, ERROR_INVALIDTOKEN);


    } else if (match_tokentypes(p, TOK_EOF, MATCH_SENTINEL)) {
        parser_throw_error(p, ERROR_EXPECTEDEXPR);


    } else if (match_tokentypes(p, TOK_LITERAL_IDENTIFIER, TOK_LITERAL_INTEGER, TOK_LITERAL_FLOAT, TOK_LITERAL_STRING, TOK_LITERAL_TRUE, TOK_LITERAL_FALSE, TOK_LITERAL_NIL, MATCH_SENTINEL)) {

        ExprLiteral l = { .operator = get_current_token(p) };
        new = ast_create_node(TYPE_LITERAL, &l);
        p->current++;

    } else {
        parser_throw_error(p, ERROR_UNEXPECTEDTOKEN);
    }

    return new;

}



AstNode* rule_unary(Parser *p) {
    // BNF: unary -> ( "!" | "-" | "/" ) unary | primary

    AstNode *new = NULL;

    if (match_tokentypes(p, TOK_UNR_BANG, TOK_BIN_UNR_MINUS, TOK_BIN_UNR_SLASH, MATCH_SENTINEL)) {

        Token operator = get_current_token(p);
        p->current++;
        AstNode *node = rule_unary(p);

        ExprUnaryOp op = { .node     = node,
                           .operator = operator };
        new = ast_create_node(TYPE_UNARYOP, &op);

    } else
        new = rule_primary(p);

    return new;

}



AstNode* rule_factor(Parser *p) {
    // BNF: factor -> unary ( ( "/" | "*" ) unary )*
    BINARYEXPR(rule_unary, TOK_BIN_UNR_SLASH, TOK_BIN_ASTERISK)
}

AstNode* rule_term(Parser *p) {
    // BNF: term -> factor ( ( "-" | "+" ) factor )*
    BINARYEXPR(rule_factor, TOK_BIN_UNR_MINUS, TOK_BIN_PLUS)
}

AstNode* rule_comparison(Parser *p) {
    // BNF: comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
    BINARYEXPR(rule_term, TOK_BIN_GREATER_THAN, TOK_GREATER_THAN_EQUAL, TOK_BIN_LESS_THAN, TOK_LESS_THAN_EQUAL)
}

AstNode* rule_equality(Parser *p) {
    // BNF: equality -> comparison ( ( "!=" | "==" ) comparison )*
    BINARYEXPR(rule_comparison, TOK_BIN_EQUAL, TOK_BIN_EQUAL_NOT)
}

AstNode* rule_expression(Parser *p) {
    // BNF: expression -> equality
    return rule_equality(p);
}


/* -- ----------- -- */








/* -- STATEMENTS -- */


AstNode* rule_idtype_pair(Parser *p) {
    // BNF: idtypepair -> IDENTIFIER "'" DATATYPE
    // eg: foo'int;

    if (!match_tokentypes(p, TOK_LITERAL_IDENTIFIER))
        parser_throw_customerror("expected identifier");

    Token identifier = get_current_token(p);
    p->current++;

    if (!match_tokentypes(p, TOK_SINGLEQUOTE, MATCH_SENTINEL))
        parser_throw_customerror("type specifier missing");

    p->current++;

    Token datatype = get_current_token(p);
    if (!is_datatype(datatype.type))
        parser_throw_customerror("invalid type annotation");

    p->current++;

    IdTypePair op = { .identifier = identifier, .type = datatype };
    return ast_create_node(TYPE_IDTYPEPAIR, &op);

}




AstNode* rule_block(Parser *p) {
    // BNF: block -> "{" statement* "}"
    // aka: scope

    if (match_tokentypes(p, TOK_LBRACE, MATCH_SENTINEL)) {
        p->current++;

        AstNodeList list;
        astnodelist_init(&list);

        while (!match_tokentypes(p, TOK_RBRACE, MATCH_SENTINEL) && !match_tokentypes(p, TOK_EOF, MATCH_SENTINEL)) {
            AstNode *stmt = rule_statement(p);
            astnodelist_append(&list, &stmt);
        }

        // check for matching brace
        if (!match_tokentypes(p, TOK_RBRACE, MATCH_SENTINEL))
            parser_throw_customerror("unmatched brace");

        p->current++;

        Block op = { list };
        return ast_create_node(TYPE_BLOCK, &op);

    } else
        return NULL;

}








// returns NULL if statement was not matched
AstNode* rule_vardeclaration(Parser *p) {
    // BNF: vardeclaration -> "let" IDTYPEPAIR ( "=" expression )? ";"

    if (match_tokentypes(p, TOK_KEYWORD_LET, MATCH_SENTINEL)) {
        ++p->current;

        AstNode *idtypepair = rule_idtype_pair(p);

        AstNode *value = NULL;
        if (match_tokentypes(p, TOK_ASSIGN, MATCH_SENTINEL)) {
            ++p->current;
            value = rule_expression(p);
        }

        // check for semicolon
        if (!match_tokentypes(p, TOK_SEMICOLON, MATCH_SENTINEL))
            parser_throw_error(p, ERROR_EXPECTEDSEMICOLON);
        ++p->current;

        StmtVarDeclaration op = { .idtypepair = idtypepair,
                                  .value      = value };

        return ast_create_node(TYPE_VARDECLARATION, &op);

    } else
        return NULL;

}


// TODO: refactor this into an expression, that takes an expression as an lvalue, while not evaluating it
AstNode* rule_assign(Parser *p) {
    // BNF: assign -> IDENTIFIER "=" expression ";"

    if (p->tokens.tokens[p->current+1].type != TOK_ASSIGN) return NULL; // TODO: this

    Token identifier = get_current_token(p);
    p->current++;

    if (!match_tokentypes(p, TOK_ASSIGN, MATCH_SENTINEL))
        return NULL;

    Token operand = get_current_token(p); // TODO: this
    p->current++;

    AstNode *expr = rule_expression(p);

    // check for semicolon
    if (!match_tokentypes(p, TOK_SEMICOLON, MATCH_SENTINEL))
        parser_throw_error(p, ERROR_EXPECTEDSEMICOLON);
    p->current++;


    StmtAssign op = { .identifier = identifier, .value = expr };
    return ast_create_node(TYPE_ASSIGN, &op);

}



AstNode* rule_exprstatement(Parser *p) {
// BNF: expressionstatement -> expression ";"
// eg: function call: evaluates and has possible side effect

    AstNode *new = rule_expression(p);

    // check for semicolon
    if (!match_tokentypes(p, TOK_SEMICOLON, MATCH_SENTINEL))
        parser_throw_error(p, ERROR_EXPECTEDSEMICOLON);
    p->current++;

    return new;
}


AstNode *rule_if(Parser *p) {
// BNF: if -> "if" expression block ("else" block)?

    if (match_tokentypes(p, TOK_KEYWORD_IF, MATCH_SENTINEL)) {
        ++p->current;

        AstNode *expr = rule_expression(p);
        AstNode *if_block = rule_block(p);
        if (if_block == NULL)
            parser_throw_customerror("expected block after `if`!");

        AstNode *else_block = NULL;
        if (match_tokentypes(p, TOK_KEYWORD_ELSE, MATCH_SENTINEL)) {
            ++p->current;
            else_block = rule_block(p);

            if (else_block == NULL)
                parser_throw_customerror("expected block after `else`!");
        }

        StmtIf op = { .condition = expr, .if_body = if_block, .else_body = else_block };
        return ast_create_node(TYPE_IF, &op);

    }
    else
        return NULL;

}


AstNode *rule_return(Parser *p) {
// BNF: return -> "ret" expression? ";"

    if (match_tokentypes(p, TOK_KEYWORD_RETURN, MATCH_SENTINEL)) {
        ++p->current;

        AstNode *expr = NULL;
        if (!match_tokentypes(p, TOK_SEMICOLON, MATCH_SENTINEL))
            expr = rule_expression(p);

        // check for semicolon
        if (!match_tokentypes(p, TOK_SEMICOLON, MATCH_SENTINEL))
            parser_throw_error(p, ERROR_EXPECTEDSEMICOLON);
        p->current++;

        StmtReturn op = { expr };
        return ast_create_node(TYPE_RETURN, &op);

    }
    else return NULL;

}


AstNode *rule_function(Parser *p) {
// BNF: function -> "defun" IDENTIFIER "(" (idtypepair ("," idtypepair)*)? ")" "->" DATATYPE body

    if (match_tokentypes(p, TOK_KEYWORD_DEFUN, MATCH_SENTINEL)) {
        ++p->current;

        if (!match_tokentypes(p, TOK_LITERAL_IDENTIFIER))
            parser_throw_customerror("expected identifier");

        Token identifier = get_current_token(p);
        ++p->current;

        if (!match_tokentypes(p, TOK_LPAREN, MATCH_SENTINEL))
            parser_throw_error(p, ERROR_UNMATCHEDPAREN);
        ++p->current;


        AstNodeList arglist;
        astnodelist_init(&arglist);

        if (match_tokentypes(p, TOK_LITERAL_IDENTIFIER, MATCH_SENTINEL)) {
            while (1) {
                AstNode *new = rule_idtype_pair(p);
                astnodelist_append(&arglist, &new);

                if (match_tokentypes(p, TOK_COMMA, MATCH_SENTINEL)) {
                    ++p->current;
                    continue;
                }
                else break;
            }
        }
        else arglist.nodes = NULL;


        if (!match_tokentypes(p, TOK_RPAREN, MATCH_SENTINEL))
            parser_throw_error(p, ERROR_UNMATCHEDPAREN);
        ++p->current;



        if (!match_tokentypes(p, TOK_ARROW))
            parser_throw_customerror("expected arrow");
        ++p->current;


        Token returntype = get_current_token(p);
        if (!is_datatype(returntype.type))
            parser_throw_customerror("expected returntype");
        ++p->current;

        AstNode *body = rule_block(p);
        if (body == NULL)
            parser_throw_customerror("expected block");


        StmtFunction op = { .identifier = identifier,
                            .body       = body,
                            .returntype = returntype,
                            .arguments  = arglist };
        return ast_create_node(TYPE_FUNCTION, &op);

    }
    else return NULL;

}



AstNode* rule_statement(Parser *p) {
// BNF: statement -> vardecl | assign | expressionstatement | block

    AstNode *new;

    new = rule_vardeclaration(p);
    if (new != NULL) return new;

    new = rule_if(p);
    if (new != NULL) return new;

    new = rule_function(p);
    if (new != NULL) return new;

    new = rule_return(p);
    if (new != NULL) return new;

    new = rule_assign(p);
    if (new != NULL) return new;

    new = rule_block(p);
    if (new != NULL) return new;

    new = rule_exprstatement(p);

    return new;

}



AstNode* rule_program(Parser *p) {
// BNF: program -> statement* EOF

    AstNodeList statements;
    astnodelist_init(&statements);

    while (!match_tokentypes(p, TOK_EOF, MATCH_SENTINEL)) {
        AstNode *new = rule_statement(p);
        astnodelist_append(&statements, &new);
    }

    ProgramRoot root = { statements };
    AstNode *node = ast_create_node(TYPE_PROGRAMROOT, &root);
    return node;

}





//--------------------------------------------------------------------------------



// FIX: provide all errors at once


AstNode*
parse(TokenList tokens, const char *filename) {

    Parser p = {
        .tokens = tokens,
        .current = 0,
        .filename = filename,
        .error = false,
    };

    AstNode *root = rule_program(&p);

    // if (p.error) exit(1);

    return root;

}
