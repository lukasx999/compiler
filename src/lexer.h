#pragma once
#ifndef _LEXER_H
#define _LEXER_H

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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
KEYWORD_ELSE    = "else",
KEYWORD_END     = "end",
KEYWORD_IN      = "in";

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
    KEYWORD_TYPE_BOOL,
    KEYWORD_ELSE,
    KEYWORD_LOOP,
    KEYWORD_TYPE_STR,
    KEYWORD_TYPE_INT,
    LITERAL_NIL,
    KEYWORD_END,
    KEYWORD_LET,
    KEYWORD_IF,
    KEYWORD_IN,

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
    TOK_ARROW,

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
    TOK_KEYWORD_ELSE,
    TOK_KEYWORD_END,
    TOK_KEYWORD_IN,

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

    "eof",
    "invalid",

    "whitespace",
    "newline",
    "semicolon",
    "dot",
    "comma",
    "pipe",
    "ampersand",
    "singlequote",
    "lparen", "rparen",
    "lbrace", "rbrace",
    "lbracket", "rbracket",

    "less_than_equal", "greater_than_equal",
    "expr_plus_equal", "expr_minus_equal",
    "assign",
    "arrow",

    "equal", "equal_not",
    "slash",
    "asterisk",
    "less_than", "greater_than",
    "plus",
    "minus",
    "and",
    "or",

    "bang",

    "let",
    "mut",
    "puts",
    "defun", "return",
    "loop",
    "if", "else",
    "end",
    "in",
    "int",
    "str",
    "bool",
    "void",

    "true",
    "false",
    "nil",
    "string",
    "integer",
    "float",
    "identifier",

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




/* -- Dynamic Array to store tokens -- */

typedef struct {

    size_t cap;
    size_t size;  // always one more than actual index
    Token *tokens;

} TokenList;


extern void tokenlist_init  (TokenList *tl),
            tokenlist_append(TokenList *tl, Token new),
            tokenlist_free  (TokenList *tl);

/* -- ----------------------------- -- */





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

// turn a string of input source code into a list of tokens
extern TokenList tokenize(const char *source);


/* -- ------------------ -- */



#endif // _LEXER_H
