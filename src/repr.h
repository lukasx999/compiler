#pragma once
#ifndef _REPR_H
#define _REPR_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "parser.h"
#include "lexer.h"
#include "colors.h"
#include "symboltable.h"


#define TOKENS_COLPOS 12

#define AST_PRINT_SPACING 2
#define AST_SEPERATOR     "⋅"


#define PRINT_COLOR(color, string) \
    printf("%s%s%s\n", (color), (string), COLOR_END)



// extern void print_symboltable    (Table *root);
extern void print_input          (const char *input);
extern void print_tokens_columns (TokenList tokens);
extern void print_tokens_stream  (TokenList tokens);
extern void print_ast            (AstNode *root);



#endif // _REPR_H
