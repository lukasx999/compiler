#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "repr.h"
#include "lexer.h"
#include "parser.h"



static void _rec_print_ast(AstNode *root, uint32_t level) {

    if (root == NULL) return;

    // indenting
    uint32_t indentlevel = level * AST_PRINT_SPACING;
    while(indentlevel--)
        printf("%s%s%s", COLOR_GRAY, AST_SEPERATOR, COLOR_END);


    // -- COLORS --
    // root:       white, underline
    // binaryop:   red
    // unaryop:    purple
    // statements: blue, bold
    // literals:   gray
    // groupings:  yellow
    // blocks:     yellow



    switch (root->type)
    {
        case TYPE_BINARYOP: {
            PRINT_COLOR(COLOR_RED, token_repr[root->ast_binaryop.operator.type]);
            _rec_print_ast(root->ast_binaryop.left_node, ++level);
            _rec_print_ast(root->ast_binaryop.right_node, level);
        } break;

        case TYPE_UNARYOP: {
            PRINT_COLOR(COLOR_PURPLE, token_repr[root->ast_unaryop.operator.type]);
            _rec_print_ast(root->ast_unaryop.node, ++level);
        } break;

        case TYPE_LITERAL: {
            if (root->ast_literal.operator.type == TOK_LITERAL_STRING) // show string literal in quotes
                printf("%s%s%s: \"%s\"%s\n", COLOR_WHITE, COLOR_ITALIC, token_repr[root->ast_literal.operator.type], root->ast_literal.operator.value, COLOR_END);
            else
                if (root->ast_literal.operator.type == TOK_LITERAL_TRUE || root->ast_literal.operator.type == TOK_LITERAL_FALSE || root->ast_literal.operator.type == TOK_LITERAL_NIL)
                    printf("%s%s%s%s\n", COLOR_WHITE, COLOR_ITALIC, token_repr[root->ast_literal.operator.type], COLOR_END);
                else
                    printf("%s%s%s: %s%s\n", COLOR_WHITE, COLOR_ITALIC, token_repr[root->ast_literal.operator.type], root->ast_literal.operator.value, COLOR_END);

        } break;


        case TYPE_GROUPING: {
            PRINT_COLOR(COLOR_YELLOW, "grouping");
            _rec_print_ast(root->ast_grouping.node, ++level);
        } break;


        case TYPE_VARDECLARATION: {
            const char *mutable_str = "";
            if (root->ast_vardecl.mutable)
                mutable_str = "(mutable)";

            printf("%svardecl: `%s` of `%s` %s%s\n", COLOR_BLUE, root->ast_vardecl.idtypepair->ast_idtypepair.identifier.value, token_repr[root->ast_vardecl.idtypepair->ast_idtypepair.type.type], mutable_str, COLOR_END);
            _rec_print_ast(root->ast_vardecl.value, ++level);
        } break;

        case TYPE_IF: {
            PRINT_COLOR(COLOR_BLUE, "if");
            _rec_print_ast(root->ast_if.condition, ++level);
            _rec_print_ast(root->ast_if.then_body,     level);
            _rec_print_ast(root->ast_if.else_body,   level);
        } break;

        case TYPE_FUNCTION: {
            printf("%sfunction: `%s` -> `%s`%s\n", COLOR_BLUE, root->ast_function.identifier.value, token_repr[root->ast_function.returntype.type], COLOR_END);

            size_t size = root->ast_function.parameters.size;

            level++;
            for (size_t i=0; i<size; ++i) {
                AstNode *node = root->ast_function.parameters.nodes[i];
                _rec_print_ast(node, level);
            }

            AstNode *node = root->ast_function.body;
            _rec_print_ast(node, level);
        } break;

        case TYPE_RETURN: {
            printf("%sreturn%s\n", COLOR_BLUE, COLOR_END);
            _rec_print_ast(root->ast_return.expression, ++level);
        } break;

        case TYPE_ASSIGN: {
            printf("%sassign%s\n", COLOR_BLUE, COLOR_END);
            _rec_print_ast(root->ast_assign.identifier, ++level);
            _rec_print_ast(root->ast_assign.value, level);
        } break;

        case TYPE_PROGRAMROOT: {
            printf("%s%s%s\n", COLOR_UNDERLINE, "root", COLOR_END);

            size_t size = root->ast_programroot.statements.size;

            level++;
            for (size_t i=0; i<size; ++i) {
                AstNode *node = root->ast_programroot.statements.nodes[i];
                _rec_print_ast(node, level);
            }

        } break;

        case TYPE_BLOCK: {
            printf("%s%s%s\n", COLOR_PURPLE_HIGH, "block", COLOR_END);

            size_t size = root->ast_block.statements.size;

            level++;
            for (size_t i=0; i<size; ++i) {
                AstNode *node = root->ast_block.statements.nodes[i];
                _rec_print_ast(node, level);
            }

        } break;

        case TYPE_IDTYPEPAIR: {
            printf("%sidtypepair `%s` of `%s`%s\n", COLOR_YELLOW_HIGH, root->ast_idtypepair.identifier.value, token_repr[root->ast_idtypepair.type.type], COLOR_END);
        } break;


        default: {
            assert(!"non-existant type");
        } break;

    }

}


void print_ast(AstNode *root) {
    puts("\n\n\n=== AST: ===\n");
    _rec_print_ast(root, 0);
    puts("\n-----------\n");
}


void print_input(const char *input) {
    printf("\n=== INPUT: ===\n\n%s%s%s\n\n--------------", COLOR_CYAN, input, COLOR_END);
}


void print_tokens_columns(TokenList tokens) {

    puts("\n\n\n=== TOKENS: ===\n");

    puts("== TYPE | LINE:COLUMN | VALUE ==\n");

    for (size_t i=0; i<tokens.size; ++i) {
        enum TokenType type = tokens.tokens[i].type;
        char *value = tokens.tokens[i].value;
        size_t column = tokens.tokens[i].column;
        size_t line = tokens.tokens[i].line_number;



        printf("%s%s%s", COLOR_CYAN, token_repr[type], COLOR_END);


        // allign columns correctly
        int offset = TOKENS_COLPOS - strlen(token_repr[type]);
        while(offset--) printf(" ");



        printf("| %s%lu:%lu%s", COLOR_GRAY, line, column, COLOR_END);

        const char *quotes = "";
        if (type == TOK_LITERAL_STRING)
            quotes = "\"";

        printf(" | %s%s%s%s%s\n", COLOR_RED, quotes, value, quotes, COLOR_END);

    }

    puts("\n== -------------------------- ==\n\n");

}

void print_tokens_stream(TokenList tokens) {

    puts("\n\n\n=== TOKENS: ===\n");

    for (size_t i=0; i<tokens.size; ++i) {
        enum TokenType type = tokens.tokens[i].type;
        char *value = tokens.tokens[i].value;

        printf("(%s%s%s", COLOR_CYAN, token_repr[type], COLOR_END);

        const char *quotes = "";
        if (type == TOK_LITERAL_STRING)
            quotes = "\"";

        if (strcmp(value, ""))
            printf(": %s%s%s%s%s) ", COLOR_RED, quotes, value, quotes, COLOR_END);
        else
            printf(") ");

    }

    puts("\n\n== -------------------------- ==\n\n");

}
