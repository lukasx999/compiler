#include <stdio.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

#include "lexer.h"



void tokenlist_init(TokenList *tl) {
    tl->cap = 5;
    tl->size = 0;
    tl->tokens = malloc(tl->cap * sizeof (Token));
}

void tokenlist_append(TokenList *tl, Token new) {

    if (tl->size+1 == tl->cap) {
        tl->cap *= 2;
        tl->tokens = realloc(tl->tokens, tl->cap * sizeof (Token));
    }

    tl->tokens[tl->size++] = new;

}

void tokenlist_free(TokenList *tl) {
    free(tl->tokens);
}






// Helper functions

static bool is_identifier(char c) {

    // (uppercase)                || (lowercase)            || '_'
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';

}

static bool is_number(char c) {

    return c >= '0' && c <= '9';

}


// matches the next word to every keyword
// returns the amount of bytes (chars) to be skipped to the last matched char (+1 when loop ends -> next char to be tokenized)
// returns 0 if no keyword matched
// output: matched_kw -> matched keyword
// if no match: matched_kw = ""

static size_t get_keyword
(char **matched_kw, Lexer *l, keyword_t *keywords, size_t keywords_size) {

    // make all keywords require at least one whitespace
    // TODO: if(foo) vs if (foo)

    size_t skip = 0;
    for (size_t i=l->pos ;; ++i) {

        char c = l->src[i];
        if (!is_identifier(c)) {
            skip = i;
            break;
        }

    }

    size_t diff = skip - l->pos;

    char buf[BUFSIZE] = { 0 };
    strncpy(buf, &l->src[l->pos], diff);

    // match against all keywords
    for (size_t i=0; i<keywords_size; ++i) {

        keyword_t keyword = keywords[i];

        if(!strcmp(keyword, buf)) { // match
            *matched_kw = (char*) keywords[i];
            return skip;
        }

    }


    // if no keyword matched
    *matched_kw = "";
    return 0;

}



void calculate_token_position(Token *new, const char *src, size_t pos) {

    new->absolute_pos = pos + 1;

    size_t newline_counter = 1, newline_pos = 0;

    for (size_t j = 0; j != pos; ++j) {

        if (src[j] == '\n') {
            ++newline_counter;
            newline_pos = j;
        }

    }

    new->line_number = newline_counter;
    new->column = new->absolute_pos - newline_pos;

}


// -----------



static enum TokenType match_keyword(char *query) {

    enum TokenType type;

    if      (!strcmp(query, KEYWORD_LET))       type = TOK_KEYWORD_LET;
    else if (!strcmp(query, KEYWORD_DEFUN))     type = TOK_KEYWORD_DEFUN;
    else if (!strcmp(query, KEYWORD_RETURN))    type = TOK_KEYWORD_RETURN;
    else if (!strcmp(query, KEYWORD_LOOP))      type = TOK_KEYWORD_LOOP;
    else if (!strcmp(query, KEYWORD_PUTS))      type = TOK_KEYWORD_PUTS;
    else if (!strcmp(query, KEYWORD_END))       type = TOK_KEYWORD_END;
    else if (!strcmp(query, KEYWORD_IF))        type = TOK_KEYWORD_IF;
    else if (!strcmp(query, KEYWORD_ELSE))      type = TOK_KEYWORD_ELSE;
    else if (!strcmp(query, KEYWORD_IN))        type = TOK_KEYWORD_IN;
    else if (!strcmp(query, LITERAL_TRUE))      type = TOK_LITERAL_TRUE;
    else if (!strcmp(query, LITERAL_FALSE))     type = TOK_LITERAL_FALSE;
    else if (!strcmp(query, LITERAL_NIL))       type = TOK_LITERAL_NIL;
    else if (!strcmp(query, KEYWORD_TYPE_INT))  type = TOK_KEYWORD_DATATYPE_INT;
    else if (!strcmp(query, KEYWORD_TYPE_STR))  type = TOK_KEYWORD_DATATYPE_STR;
    else if (!strcmp(query, KEYWORD_TYPE_VOID)) type = TOK_KEYWORD_DATATYPE_VOID;

    else assert(!"keyword case not handled");

    return type;

}



// looks ahead if the next char is equal to `c`
// returns true if token matched, otherwise false
bool lexer_lookahead(Lexer *l, char c) {
    return l->src[l->pos+1] == c;
}


bool lexer_is_at_end(Lexer *l) {
    return l->pos == l->src_size;
}


Token lexer_next_token(Lexer *l, bool *skip_token) {

    Token new;
    memset(new.value, 0, BUFSIZE);

    const char c = l->src[l->pos];

    size_t skip = 0;  // determines how many chars should be skipped (only for keywords, strings, etc ...)

    switch (c) {

        // SINGLE CHAR SYMBOLS
        case PUNCT_NULLBYTE:
            // *skip_token = true;
            new.type = TOK_EOF;
            break;

        case PUNCT_NEWLINE:
            // new.type = TOK_NEWLINE;
            *skip_token = true;
            break;

        case PUNCT_WHITESPACE:
            // new.type = TOK_WHITESPACE;
            *skip_token = true;
            break;

        case PUNCT_SINGLEQUOTE:
            new.type = TOK_SINGLEQUOTE;
            break;

        case PUNCT_DOT:
            new.type = TOK_DOT;
            break;

        case PUNCT_ASTERISK:
            new.type = TOK_BIN_ASTERISK;
            break;

        case PUNCT_AMPERSAND:
            new.type = TOK_AMPERSAND;
            break;

        case PUNCT_SEMICOLON:
            new.type = TOK_SEMICOLON;
            break;

        case PUNCT_COMMA:
            new.type = TOK_COMMA;
            break;

        case PUNCT_LPAREN:
            new.type = TOK_LPAREN;
            break;

        case PUNCT_RPAREN:
            new.type = TOK_RPAREN;
            break;

        case PUNCT_LBRACE:
            new.type = TOK_LBRACE;
            break;

        case PUNCT_RBRACE:
            new.type = TOK_RBRACE;
            break;

        case PUNCT_LBRACKET:
            new.type = TOK_LBRACKET;
            break;

        case PUNCT_RBRACKET:
            new.type = TOK_RBRACKET;
            break;

        case PUNCT_SLASH:
            new.type = TOK_BIN_UNR_SLASH;
            break;




        // DOUBLE CHAR SYMBOLS

        case PUNCT_EQUAL: {
            if (lexer_lookahead(l, PUNCT_EQUAL)) {
                new.type = TOK_BIN_EQUAL;
                ++l->pos;

            } else
                new.type = TOK_ASSIGN;

        } break;

        case '!': {
            if (lexer_lookahead(l, PUNCT_EQUAL)) {
                new.type = TOK_BIN_EQUAL_NOT;
                ++l->pos;

            } else
                new.type = TOK_UNR_BANG;

        } break;

        case '>': {
            if (lexer_lookahead(l, PUNCT_EQUAL)) {
                new.type = TOK_GREATER_THAN_EQUAL;
                ++l->pos;

            } else
                new.type = TOK_BIN_GREATER_THAN;

        } break;

        case '<': {
            if (lexer_lookahead(l, PUNCT_EQUAL)) {
                new.type = TOK_LESS_THAN_EQUAL;
                ++l->pos;

            } else
                new.type = TOK_BIN_LESS_THAN;

        } break;

        case '+': {
            if (lexer_lookahead(l, PUNCT_EQUAL)) {
                new.type = TOK_EXPR_PLUS_EQUAL;
                ++l->pos;

            } else
                new.type = TOK_BIN_PLUS;

        } break;

        case '-': {
            if (lexer_lookahead(l, '>')) {
                new.type = TOK_ARROW;
                ++l->pos;

            } else if (lexer_lookahead(l, PUNCT_EQUAL)) {
                new.type = TOK_EXPR_MINUS_EQUAL;
                ++l->pos;

            } else
                new.type = TOK_BIN_UNR_MINUS;

        } break;




        // DOUBLE QUOTED STRINGS
        // case PUNCT_STRING_QUOTE_SINGLE:
        case PUNCT_STRING_QUOTE_DOUBLE: {

            // char that was matched above
            // char match = (c == PUNCT_STRING_QUOTE_SINGLE ? PUNCT_STRING_QUOTE_SINGLE : PUNCT_STRING_QUOTE_DOUBLE);
            char match = c;


            for (size_t j = l->pos+1 ;; ++j) {
                char chr = l->src[j];

                if (chr == match) {
                    skip = j;
                    break;
                }

                if (chr == PUNCT_NULLBYTE) {  // input string is terminated with nullbyte
                    skip = 0;
                    break;
                }

            }

            if (skip) {

                size_t diff = skip - l->pos;
                strncpy(new.value, &l->src[l->pos+1], diff-1);  // +-1 to ignore quotes
                new.type = TOK_LITERAL_STRING;

                l->pos = skip;

            } else {
                // unterminated string
                new.type = TOK_INVALID;
                new.value[0] = c;
            }

        } break;




        // LITERALS
        default: {




            /* KEYWORDS */
            if (is_identifier(c)) {

                size_t keywords_size = GET_ARRAY_SIZE(keywords);
                char *matched;
                skip = get_keyword(&matched, l, keywords, keywords_size);

                if (skip) {

                    new.type = match_keyword(matched);
                    l->pos = skip-1;  // skip to the end of the keyword
                    break;  // break out of "default:"

                }

            }



            /* IDENTIFIERS */
            if (is_identifier(c)) {

                for (size_t j = l->pos+1 ;; ++j) {
                    char chr = l->src[j];

                    if (!is_identifier(chr) && !is_number(chr)) {
                        skip = j;
                        break;
                    }
                }

                size_t diff = skip - l->pos;
                strncpy(new.value, &l->src[l->pos], diff);
                new.type = TOK_LITERAL_IDENTIFIER;

                l->pos = --skip;

            }





            /* INTEGER LITERALS */
            else if (is_number(c)) {

                bool is_float = false;

                for (size_t j = l->pos+1 ;; ++j) {
                    char chr = l->src[j];
                    char next = l->src[j+1];

                    // floats
                    if (chr == PUNCT_FLOAT_DELIM && is_number(next)) // floats with a trailing dot are not allowed: correct float: 10.0, 12.3, incorrect float: 10.
                        is_float = true;

                    else if (!is_number(chr)) {
                        skip = j;
                        break;
                    }

                }

                size_t diff = skip - l->pos;
                strncpy(new.value, &l->src[l->pos], diff);

                if (is_float)
                    new.type = TOK_LITERAL_FLOAT;
                else
                    new.type = TOK_LITERAL_INTEGER;

                l->pos = --skip;

            } else {
                // invalid/unknown character
                new.type = TOK_INVALID;
                new.value[0] = c;
            }

        } break;

    }

    ++l->pos;  // move the cursor forwards
    return new;

}





TokenList tokenize(const char *source) {

    TokenList tokens;
    tokenlist_init(&tokens);

    Lexer lexer = {
        .src = source,
        .src_size = strlen(source),
        .pos = 0,
    };

    while (!lexer_is_at_end(&lexer)) {

        bool skip_token = false;  // will not append current ´new´ token if true

        size_t token_start = lexer.pos; // save the start position of the token
        Token new = lexer_next_token(&lexer, &skip_token);
        calculate_token_position(&new, source, token_start);

        new.length = lexer.pos - token_start;

        if (skip_token) continue;
        tokenlist_append(&tokens, new);

    }

    // append eof token at the end
    // TODO: refactor this
    Token eof = { .type = TOK_EOF };
    calculate_token_position(&eof, source, lexer.pos);
    memset(eof.value, 0, BUFSIZE);
    tokenlist_append(&tokens, eof);

    return tokens;

}
