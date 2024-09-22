#pragma once
#ifndef _REPR_H
#define _REPR_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "parser.h"
#include "lexer.h"


#define TOKENS_COLPOS 12

#define AST_PRINT_SPACING 3
#define AST_SEPERATOR     "⋅"

#define COLOR_RED         "\33[31m"
#define COLOR_BLUE        "\33[34m"
#define COLOR_PURPLE      "\33[35m"
#define COLOR_GRAY        "\33[90m"
#define COLOR_CYAN        "\33[36m"
#define COLOR_YELLOW      "\33[33m"
#define COLOR_WHITE       "\33[97m"
#define COLOR_GREEN       "\33[32m"

// high intensity
#define COLOR_BLUE_HIGH   "\33[94m"
#define COLOR_YELLOW_HIGH "\33[93m"
#define COLOR_PURPLE_HIGH "\33[95m"

#define COLOR_BOLD        "\33[1m"
#define COLOR_UNDERLINE   "\33[4m"
#define COLOR_ITALIC      "\33[3m"

#define COLOR_END         "\33[0m"


extern void print_input  (const char *input);
extern void print_tokens (TokenList tokens);
extern void print_ast    (AstNode *root);


#endif // _REPR_H
