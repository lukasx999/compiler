#pragma once
#ifndef _LEXER_H
#define _LEXER_H

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "vector.h"

#define BUFSIZE 100
#define GET_ARRAY_SIZE(array) (sizeof((array)) / sizeof(*(array)))


#define PUNCT_WHITESPACE          ' '
#define PUNCT_FLOAT_DELIM         '.'
#define PUNCT_DOT                 '.'
#define PUNCT_NEWLINE             '\n'
#define PUNCT_SEMICOLON           ';'
#define PUNCT_AMPERSAND           '&'
#define PUNCT_PIPE                '|'
#define PUNCT_ASTERISK            '*'
#define PUNCT_COMMA               ','
#define PUNCT_EQUAL               '='
#define PUNCT_STRING_QUOTE_DOUBLE '\"'
#define PUNCT_LPAREN              '('
#define PUNCT_SLASH               '/'
#define PUNCT_RPAREN              ')'
#define PUNCT_LBRACE              '{'
#define PUNCT_RBRACE              '}'
#define PUNCT_LBRACKET            '['
#define PUNCT_RBRACKET            ']'
#define PUNCT_SINGLEQUOTE         '\''
#define PUNCT_DOLLARSIGN          '$'
#define PUNCT_NULLBYTE            '\0'


typedef const char * const keyword_t;

static keyword_t
KEYWORD_LET     = "let",
KEYWORD_MUT     = "mut",
KEYWORD_DEFUN   = "defun",
KEYWORD_RETURN  = "ret",
KEYWORD_PUTS    = "puts",
KEYWORD_LOOP    = "loop",
KEYWORD_IF      = "if",
KEYWORD_ELSEIF  = "elsif",
KEYWORD_ELSE    = "else";

// these are actually literals, but we'll treat them as keywords
static keyword_t
LITERAL_TRUE      = "true",
LITERAL_FALSE     = "false",
LITERAL_NIL       = "nil",
KEYWORD_TYPE_INT  = "int",
KEYWORD_TYPE_STR  = "str",
KEYWORD_TYPE_BOOL = "bool",
KEYWORD_TYPE_VOID = "void";


// keywords always have to be manually sorted by length in descending order (eg: for vs foreach -> matches only for)
static keyword_t keywords[] = {
    LITERAL_FALSE,
    KEYWORD_DEFUN,
    KEYWORD_RETURN,
    LITERAL_TRUE,
    KEYWORD_PUTS,
    KEYWORD_TYPE_VOID,
    KEYWORD_MUT,
    KEYWORD_ELSEIF,
    KEYWORD_TYPE_BOOL,
    KEYWORD_ELSE,
    KEYWORD_LOOP,
    KEYWORD_TYPE_STR,
    KEYWORD_TYPE_INT,
    LITERAL_NIL,
    KEYWORD_LET,
    KEYWORD_IF,

};



// TOK ... Token
// UNR ... Unary
// BIN ... Binary
enum TokenType {

    /*
    TODO: future tokens
    inject
    define
    set
    let
    tag / goto
    destroy
    */

    // TODO: modulus operator %

    // do not set anything to `-1` as this is the value of `MATCH_SENTINEL`

    // ETC
    TOK_EOF,
    TOK_INVALID,

    // SINGLE CHAR TOKENS
    TOK_WHITESPACE,
    TOK_NEWLINE,
    TOK_SEMICOLON,
    TOK_DOT,
    TOK_COMMA,

    TOK_PIPE, TOK_AMPERSAND,
    TOK_SINGLEQUOTE,

    TOK_LPAREN, TOK_RPAREN,
    TOK_LBRACE, TOK_RBRACE,
    TOK_LBRACKET, TOK_RBRACKET,


    // SINGLE OR DOUBLE CHAR TOKENS
    TOK_LESS_THAN_EQUAL, TOK_GREATER_THAN_EQUAL,
    TOK_EXPR_PLUS_EQUAL, TOK_EXPR_MINUS_EQUAL,
    TOK_ASSIGN,
    TOK_ARROW, TOK_DOUBLE_ARROW,

    // BINARY EXPRESSIONS
    TOK_BIN_EQUAL, TOK_BIN_EQUAL_NOT,
    TOK_BIN_UNR_SLASH,
    TOK_BIN_ASTERISK,
    TOK_BIN_LESS_THAN, TOK_BIN_GREATER_THAN,
    TOK_BIN_PLUS,
    TOK_BIN_UNR_MINUS,  // minus is both binary and unary
    TOK_BIN_LOGICAL_AND,
    TOK_BIN_LOGICAL_OR,

    // UNARY EXPRESSIONS
    TOK_UNR_BANG,

    // KEYWORDS
    TOK_KEYWORD_LET,
    TOK_KEYWORD_MUT,
    TOK_KEYWORD_PUTS,
    TOK_KEYWORD_DEFUN,
    TOK_KEYWORD_RETURN,
    TOK_KEYWORD_LOOP,
    TOK_KEYWORD_IF,
    TOK_KEYWORD_ELSEIF,
    TOK_KEYWORD_ELSE,

    TOK_KEYWORD_DATATYPE_INT,
    TOK_KEYWORD_DATATYPE_STR,
    TOK_KEYWORD_DATATYPE_BOOL,
    TOK_KEYWORD_DATATYPE_VOID,
    // TODO: floats, ...

    // LITERALS
    TOK_LITERAL_TRUE, TOK_LITERAL_FALSE,
    TOK_LITERAL_NIL,
    TOK_LITERAL_STRING,
    TOK_LITERAL_INTEGER,
    TOK_LITERAL_FLOAT,
    TOK_LITERAL_IDENTIFIER,

};

static enum TokenType datatypes[] = {
    TOK_KEYWORD_DATATYPE_INT,
    TOK_KEYWORD_DATATYPE_STR,
    TOK_KEYWORD_DATATYPE_BOOL,
    TOK_KEYWORD_DATATYPE_VOID,
};

static inline bool
is_datatype(enum TokenType type) {
    for (size_t i=0; i<GET_ARRAY_SIZE(datatypes); ++i) {
        if (type == datatypes[i])
            return true;
    }
    return false;
}


static char token_repr[][BUFSIZE] = {

    [TOK_EOF]                   = "eof",
    [TOK_INVALID]               = "invalid",

    [TOK_WHITESPACE]            = "whitespace",
    [TOK_NEWLINE]               = "newline",
    [TOK_SEMICOLON]             = "semicolon",
    [TOK_DOT]                   = "dot",
    [TOK_COMMA]                 = "comma",
    [TOK_PIPE]                  = "pipe",
    [TOK_AMPERSAND]             = "ampersand",
    [TOK_SINGLEQUOTE]           = "singlequote",
    [TOK_LPAREN]                = "lparen",
    [TOK_RPAREN]                =  "rparen",
    [TOK_LBRACE]                = "lbrace",
    [TOK_RBRACE]                = "rbrace",
    [TOK_LBRACKET]              = "lbracket",
    [TOK_RBRACKET]              = "rbracket",

    [TOK_LESS_THAN_EQUAL]       = "less_than_equal",
    [TOK_GREATER_THAN_EQUAL]    = "greater_than_equal",
    [TOK_EXPR_PLUS_EQUAL]       = "expr_plus_equal",
    [TOK_EXPR_MINUS_EQUAL]      = "expr_minus_equal",
    [TOK_ASSIGN]                = "assign",
    [TOK_ARROW]                 = "arrow",
    [TOK_DOUBLE_ARROW]          = "doublearrow",

    [TOK_BIN_EQUAL]             = "equal",
    [TOK_BIN_EQUAL_NOT]         = "equal_not",

    [TOK_BIN_UNR_SLASH]         = "slash",
    [TOK_BIN_ASTERISK]          = "asterisk",
    [TOK_BIN_LESS_THAN]         = "less_than",
    [TOK_BIN_GREATER_THAN]      = "greater_than",
    [TOK_BIN_PLUS]              = "plus",
    [TOK_BIN_UNR_MINUS]         = "minus",
    [TOK_BIN_LOGICAL_AND]       = "and",
    [TOK_BIN_LOGICAL_OR]        = "or",

    [TOK_UNR_BANG]              = "bang",

    [TOK_KEYWORD_LET]           = "let",
    [TOK_KEYWORD_MUT]           = "mut",
    [TOK_KEYWORD_PUTS]          = "puts",
    [TOK_KEYWORD_DEFUN]         = "defun",
    [TOK_KEYWORD_RETURN]        = "return",
    [TOK_KEYWORD_LOOP]          = "loop",
    [TOK_KEYWORD_IF]            = "if",
    [TOK_KEYWORD_ELSEIF]        = "elsif",
    [TOK_KEYWORD_ELSE]          = "else",
    [TOK_KEYWORD_DATATYPE_INT]  = "int",
    [TOK_KEYWORD_DATATYPE_STR]  = "str",
    [TOK_KEYWORD_DATATYPE_BOOL] = "bool",
    [TOK_KEYWORD_DATATYPE_VOID] = "void",

    [TOK_LITERAL_TRUE]          = "true",
    [TOK_LITERAL_FALSE]         = "false",
    [TOK_LITERAL_NIL]           = "nil",
    [TOK_LITERAL_STRING]        = "string",
    [TOK_LITERAL_INTEGER]       = "integer",
    [TOK_LITERAL_FLOAT]         = "float",
    [TOK_LITERAL_IDENTIFIER]    = "identifier",

};






/* -- Token Structure -- */


typedef struct {

    enum TokenType type;
    char value[BUFSIZE];  // only filled in, if type is a literal or invalid

    // position of the token (for errors/warnings)
    size_t absolute_pos;
    size_t line_number;
    size_t column;

    // length of the token (for syntax highlighting)
    size_t length; 

} Token;


// fill in the position fields of a token, for error messages and (possibly) syntax highlighting
extern void calculate_token_position(Token *new, const char *src, size_t pos);


/* -- --------------- -- */



/* -- State of the Lexer -- */


typedef struct {

    const char *src;        // input source code as string
    const size_t src_size;  // strlen(src)
    size_t pos;             // current index in source string

} Lexer;


// matches next char
extern bool lexer_lookahead(Lexer *l, char c);

// whether position == src_size
extern bool lexer_is_at_end(Lexer *l);

// moves to the end of the next token and returns it
extern Token lexer_next_token(Lexer *l, bool *skip_token);


typedef vec_Vector TokenList;
// turn a string of input source code into a list of tokens
extern TokenList tokenize(const char *source);


/* -- ------------------ -- */



#endif // _LEXER_H
