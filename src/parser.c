#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>

#include "parser.h"
#include "colors.h"
#include "err.h"


jmp_buf g_jmp_env;


#define MAX_ERRORS 15

#define MATCH_SENTINEL -1

#define BINARYEXPR(callback, ...) {                            \
                                                               \
    AstNode *new = callback(p);                                \
                                                               \
    while (match_tokentypes(p, __VA_ARGS__, MATCH_SENTINEL)) { \
                                                               \
        Token operator = parser_get_current_token(p);          \
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


void parser_exit_errors(Parser *p) {
    // exit and show error count
    fprintf(stderr, "\nExiting with %s%s%s%d errors%s\n", COLOR_RED, COLOR_BOLD, COLOR_UNDERLINE, p->error_count, COLOR_END);
    exit(1);
}





void parser_throw_error(Parser *p, enum ErrorType type, const char *message) {

    // too many errors...
    if (p->error_count >= MAX_ERRORS) parser_exit_errors(p);
    ++p->error_count;


    // pre-defined messages
    switch (type) {
        case ERROR_EXPECTED_SEMICOLON: {
            p->current--; // INFO: we use longjmp anyway, so what does it matter?
            err_correct(parser_get_current_token(p), "expected semicolon", ";", p->source, p->filename);
            p->current++;
        } break;

        case ERROR_EXPECTED_RETURNTYPE: {
            p->current--;
            err_correct(parser_get_current_token(p), "expected returntype", " void", p->source, p->filename);
            p->current++;
        } break;

        case ERROR_EXPECTED_ARROW: {
            p->current--;
            err_correct(parser_get_current_token(p), "expected arrow", " ->", p->source, p->filename);
            p->current++;
        } break;

        case ERROR_TYPE_SPECIFIER_MISSING: {
            p->current--;
            err_correct(parser_get_current_token(p), "type specifier missing", "'int", p->source, p->filename);
            p->current++;
        } break;



        case ERROR_CUSTOM: {
            err_throw(parser_get_current_token(p), message, p->source, p->filename);
        } break;


        default: assert(!"unknown type"); break;
    }

    parser_synchronize(p);
    longjmp(g_jmp_env, 1); // return to global scope

}




// `operation` gets casted to corresponding type, according to `Node_type`
AstNode*
ast_create_node(enum AstNode_type type, void *operation) {

    AstNode *new = malloc(sizeof(AstNode));

    new->type = type;

    switch (type) {

        case TYPE_BINARYOP:       new->ast_binaryop    = *(ExprBinaryOp*)        operation; break;
        case TYPE_UNARYOP:        new->ast_unaryop     = *(ExprUnaryOp*)         operation; break;
        case TYPE_LITERAL:        new->ast_literal     = *(ExprLiteral*)         operation; break;
        case TYPE_GROUPING:       new->ast_grouping    = *(ExprGrouping*)        operation; break;
        case TYPE_ASSIGN:         new->ast_assign      = *(ExprAssign*)          operation; break;
        case TYPE_CALL:           new->ast_call        = *(ExprCall*)            operation; break;
        case TYPE_VARDECLARATION: new->ast_vardecl     = *(StmtVarDeclaration*)  operation; break;
        case TYPE_IF:             new->ast_if          = *(StmtIf*)              operation; break;
        case TYPE_FUNCTION:       new->ast_function    = *(StmtFunction*)        operation; break;
        case TYPE_RETURN:         new->ast_return      = *(StmtReturn*)          operation; break;
        case TYPE_BLOCK:          new->ast_block       = *(Block*)               operation; break;
        case TYPE_IDTYPEPAIR:     new->ast_idtypepair  = *(IdTypePair*)          operation; break;

        default: assert(!"non-existant type"); break;

    }

    return new;

}



//--------------------------------------------------------------------------------


Token parser_get_current_token(Parser *p) {
    return *(Token*) vec_get(&p->tokens, p->current);
}

Token parser_get_token_by_index(Parser *p, size_t index) {
    return *(Token*) vec_get(&p->tokens, index);
}

void parser_synchronize(Parser *p) {
    // jump to the next statement after a semicolon

    size_t new = 0;
    for (size_t i=p->current; i<p->tokens.size; ++i) {
        if (parser_get_token_by_index(p, i).type == TOK_SEMICOLON) {
            new = i;
            break;
        }
        // no more statements to parse
        else if (parser_get_token_by_index(p, i).type == TOK_EOF)
            parser_exit_errors(p);

    }

    p->current = new + 1;

}


// matches to current token in the parser against any of the supplied token types
// this is a pure convience function
bool match_tokentypes(Parser *p, ...) {

    enum TokenType input = parser_get_current_token(p).type;

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

        ExprGrouping operation = { group };
        new = ast_create_node(TYPE_GROUPING, &operation);

        // check for matched paren
        if (!match_tokentypes(p, TOK_RPAREN, MATCH_SENTINEL))
            parser_throw_error(p, ERROR_CUSTOM, "unmatched parenthesis");

        p->current++;

    } else if (match_tokentypes(p, TOK_INVALID, MATCH_SENTINEL)) {
        parser_throw_error(p, ERROR_CUSTOM, "invalid token");


    } else if (match_tokentypes(p, TOK_EOF, MATCH_SENTINEL)) {
        parser_throw_error(p, ERROR_CUSTOM, "expected expression");


    } else if (match_tokentypes(p, TOK_LITERAL_IDENTIFIER, TOK_LITERAL_INTEGER,
                                   TOK_LITERAL_FLOAT,      TOK_LITERAL_STRING,
                                   TOK_LITERAL_TRUE,       TOK_LITERAL_FALSE,
                                   TOK_LITERAL_NIL,        MATCH_SENTINEL)) {

        ExprLiteral l = { parser_get_current_token(p) };
        new = ast_create_node(TYPE_LITERAL, &l);
        p->current++;

    } else {
        parser_throw_error(p, ERROR_CUSTOM, "unexpected token");
    }

    return new;

}


AstNode* rule_call(Parser *p) {
    // BNF: call -> primary ( "(" ( expression ( "," expression )* )? ")" )*

    AstNode *new = rule_primary(p);

    while (match_tokentypes(p, TOK_LPAREN, MATCH_SENTINEL)) {
        ++p->current;

        vec_Vector arglist;
        vec_init(&arglist, sizeof(AstNode), 5, 2);

        if (!match_tokentypes(p, TOK_RPAREN, MATCH_SENTINEL)) {

            do {
                AstNode *new = rule_expression(p);
                vec_push(&arglist, new);
            }
            while (match_tokentypes(p, TOK_COMMA, MATCH_SENTINEL) && p->current++); // short-circuit!

        } else
            arglist.blob = NULL;

        if (!match_tokentypes(p, TOK_RPAREN, MATCH_SENTINEL))
            parser_throw_error(p, ERROR_CUSTOM, "closing paren missing");

        ++p->current;
        ExprCall op = { .callee = new, .arguments = arglist};
        new = ast_create_node(TYPE_CALL, &op);

    }

    return new;

}


AstNode* rule_unary(Parser *p) {
    // BNF: unary -> ( "!" | "-" | "/" ) unary | primary

    AstNode *new = NULL;

    if (match_tokentypes(p, TOK_UNR_BANG, TOK_BIN_UNR_MINUS, TOK_BIN_UNR_SLASH, MATCH_SENTINEL)) {

        Token operator = parser_get_current_token(p);
        p->current++;
        AstNode *node = rule_unary(p);

        ExprUnaryOp op = { .node     = node,
                           .operator = operator };
        new = ast_create_node(TYPE_UNARYOP, &op);

    } else
        new = rule_call(p);

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

AstNode* rule_logic_and(Parser *p) {
    // BNF: logic_and -> equality ( "and" equality )*
    BINARYEXPR(rule_equality, TOK_BIN_LOGICAL_AND)
}

AstNode* rule_logic_or(Parser *p) {
    // BNF: logic_or -> logic_and ( "||" logic_and )*
    BINARYEXPR(rule_logic_and, TOK_BIN_LOGICAL_OR)
}


AstNode* rule_assign(Parser *p) {
    // BNF: assign -> rule_logic_or "=" rule_assign | rule_logic_or

    AstNode *expr = rule_logic_or(p);
    if (match_tokentypes(p, TOK_ASSIGN, MATCH_SENTINEL)){
        ++p->current;

        AstNode *value = rule_assign(p);

        ExprAssign op = { .identifier = expr,
                          .value      = value };
        expr = ast_create_node(TYPE_ASSIGN, &op);

    }

    return expr;

}




AstNode* rule_expression(Parser *p) {
    // BNF: expression -> equality
    return rule_assign(p);
}


/* -- ----------- -- */




/* -- STATEMENTS -- */


AstNode* rule_idtype_pair(Parser *p) {
    // BNF: idtypepair -> IDENTIFIER "'" DATATYPE
    // eg: foo'int;

    if (!match_tokentypes(p, TOK_LITERAL_IDENTIFIER, MATCH_SENTINEL))
        parser_throw_error(p, ERROR_CUSTOM, "expected identifier");

    Token identifier = parser_get_current_token(p);
    p->current++;

    if (!match_tokentypes(p, TOK_SINGLEQUOTE, MATCH_SENTINEL))
        parser_throw_error(p, ERROR_TYPE_SPECIFIER_MISSING, NULL);

    p->current++;
    Token datatype = parser_get_current_token(p);
    // dont check valid types, because of custom classes
    p->current++;

    IdTypePair op = { .identifier = identifier, .type = datatype };
    return ast_create_node(TYPE_IDTYPEPAIR, &op);

}




AstNode* rule_block(Parser *p) {
    // BNF: block -> "{" statement* "}"
    // aka: scope

    if (match_tokentypes(p, TOK_LBRACE, MATCH_SENTINEL)) {
        p->current++;

        vec_Vector list;
        vec_init(&list, sizeof(AstNode), 5, 2);

        while (!match_tokentypes(p, TOK_RBRACE, MATCH_SENTINEL) && !match_tokentypes(p, TOK_EOF, MATCH_SENTINEL)) {
            AstNode *stmt = rule_statement(p);
            vec_push(&list, stmt);
        }

        // check for matching brace
        if (!match_tokentypes(p, TOK_RBRACE, MATCH_SENTINEL))
            parser_throw_error(p, ERROR_CUSTOM, "unmatched brace");

        p->current++;

        Block op = { .statements = list, .root = false };
        return ast_create_node(TYPE_BLOCK, &op);

    } else
        return NULL;

}








// returns NULL if statement was not matched
AstNode* rule_vardeclaration(Parser *p) {
    // BNF: vardeclaration -> "let" "mut"? IDTYPEPAIR ( "=" expression )? ";"

    if (match_tokentypes(p, TOK_KEYWORD_LET, MATCH_SENTINEL)) {
        ++p->current;

        bool mutable = false;
        if (match_tokentypes(p, TOK_KEYWORD_MUT, MATCH_SENTINEL)) {
            mutable = true;
            ++p->current;
        }


        AstNode *idtypepair = rule_idtype_pair(p);

        AstNode *value = NULL;
        if (match_tokentypes(p, TOK_ASSIGN, MATCH_SENTINEL)) {
            ++p->current;
            value = rule_expression(p);
        }

        // check for semicolon
        if (!match_tokentypes(p, TOK_SEMICOLON, MATCH_SENTINEL))
            parser_throw_error(p, ERROR_EXPECTED_SEMICOLON, NULL);
        ++p->current;

        StmtVarDeclaration op = { .idtypepair = idtypepair,
                                  .value      = value,
                                  .mutable    = mutable };

        return ast_create_node(TYPE_VARDECLARATION, &op);

    } else
        return NULL;

}





AstNode* rule_exprstatement(Parser *p) {
// BNF: expressionstatement -> expression ";"
// eg: function call: evaluates and has possible side effect

    AstNode *new = rule_expression(p);

    // check for semicolon
    if (!match_tokentypes(p, TOK_SEMICOLON, MATCH_SENTINEL))
        parser_throw_error(p, ERROR_EXPECTED_SEMICOLON, NULL);

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
            parser_throw_error(p, ERROR_CUSTOM, "expected block after `if`!");

        AstNode *else_block = NULL;
        if (match_tokentypes(p, TOK_KEYWORD_ELSE, MATCH_SENTINEL)) {
            ++p->current;
            else_block = rule_block(p);

            if (else_block == NULL)
                parser_throw_error(p, ERROR_CUSTOM, "expected block after `else`!");
        }

        StmtIf op = { .condition = expr, .then_body = if_block, .else_body = else_block };
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
            parser_throw_error(p, ERROR_EXPECTED_SEMICOLON, NULL);
        p->current++;

        StmtReturn op = { expr };
        return ast_create_node(TYPE_RETURN, &op);

    }
    else return NULL;

}

AstNode* rule_function(Parser *p) {
// BNF: function -> "defun" IDENTIFIER "(" (idtypepair ("," idtypepair)*)? ")" "->" DATATYPE (body | ";")

    if (match_tokentypes(p, TOK_KEYWORD_DEFUN, MATCH_SENTINEL)) {
        ++p->current;

        if (!match_tokentypes(p, TOK_LITERAL_IDENTIFIER, MATCH_SENTINEL))
            parser_throw_error(p, ERROR_CUSTOM, "expected identifier");

        Token identifier = parser_get_current_token(p);
        ++p->current;

        if (!match_tokentypes(p, TOK_LPAREN, MATCH_SENTINEL))
            parser_throw_error(p, ERROR_CUSTOM, "unmatched parenthesis");
        ++p->current;


        vec_Vector paramlist;
        vec_init(&paramlist, sizeof(AstNode), 5, 2);

        if (match_tokentypes(p, TOK_LITERAL_IDENTIFIER, MATCH_SENTINEL)) {
            while (1) {
                AstNode *new = rule_idtype_pair(p);
                vec_push(&paramlist, new);

                if (match_tokentypes(p, TOK_COMMA, MATCH_SENTINEL)) {
                    ++p->current;
                    continue;
                }
                else break;
            }
        }
        else paramlist.blob = NULL;


        if (!match_tokentypes(p, TOK_RPAREN, MATCH_SENTINEL))
            parser_throw_error(p, ERROR_CUSTOM, "unmatched parenthesis");
        ++p->current;



        if (!match_tokentypes(p, TOK_ARROW, MATCH_SENTINEL))
            parser_throw_error(p, ERROR_EXPECTED_ARROW, NULL);
        ++p->current;


        Token returntype = parser_get_current_token(p);
        if (!is_datatype(returntype.type))
            parser_throw_error(p, ERROR_EXPECTED_RETURNTYPE, NULL);

        ++p->current;

        AstNode *body;
        if (match_tokentypes(p, TOK_SEMICOLON, MATCH_SENTINEL)) {
            body = NULL;
            ++p->current;

        } else {

            body = rule_block(p);
            if (body == NULL)
                parser_throw_error(p, ERROR_CUSTOM, "expected block");
        }


        StmtFunction op = { .identifier = identifier,
                            .body       = body,
                            .returntype = returntype,
                            .parameters  = paramlist };
        return ast_create_node(TYPE_FUNCTION, &op);

    }
    else return NULL;

}



AstNode* rule_statement(Parser *p) {
// BNF: statement -> vardecl | if | function | return | block | expressionstatement

    AstNode *new;

    new = rule_vardeclaration(p);
    if (new != NULL) return new;

    new = rule_if(p);
    if (new != NULL) return new;

    new = rule_function(p);
    if (new != NULL) return new;

    new = rule_return(p);
    if (new != NULL) return new;

    new = rule_block(p);
    if (new != NULL) return new;

    new = rule_exprstatement(p);

    return new;

}




AstNode* rule_program(Parser *p) {
// BNF: program -> statement* EOF

    vec_Vector statements;
    vec_init(&statements, sizeof(AstNode), 5, 2);

    setjmp(g_jmp_env); // return to here if any errors occur

    while (!match_tokentypes(p, TOK_EOF, MATCH_SENTINEL)) {
        AstNode *new = rule_statement(p);
        vec_push(&statements, new);
    }

    Block root = { .statements = statements, .root = true };
    AstNode *node = ast_create_node(TYPE_BLOCK, &root);
    return node;

}





//--------------------------------------------------------------------------------

AstNode*
parse(TokenList tokens, char *source, const char *filename) {

    Parser p = {
        .tokens      = tokens,
        .source      = source,
        .current     = 0,
        .filename    = filename,
        .error_count = 0,
    };

    AstNode *root = rule_program(&p);

    if (p.error_count != 0)
        parser_exit_errors(&p);

    return root;

}
