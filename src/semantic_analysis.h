#pragma once
#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "lexer.h"
#include "parser.h"
#include "vector.h"

#include <stdint.h>
#include <stdbool.h>


enum Datatype {
    DATATYPE_INTEGER,
    DATATYPE_FLOAT,
    DATATYPE_STRING,
    DATATYPE_BOOL,
    DATATYPE_VOID,
    DATATYPE_FUNCTION,
};

static char datatype_repr[][BUFSIZE] = {
    [DATATYPE_INTEGER] = "int",
    [DATATYPE_FLOAT] = "float",
    [DATATYPE_STRING] = "string",
    [DATATYPE_BOOL] = "bool",
    [DATATYPE_VOID] = "void",
    [DATATYPE_FUNCTION] = "func",
};







// Symbol table tree

typedef struct {
    char *identifier;
    char *value;
    enum Datatype type;

    bool not_initialized;

    // Token token;
} TableColumn; // column of the symbol table

typedef vec_Vector TableRows; // array of columns
typedef vec_Vector NodeList;  // array of pointers to several child nodes

typedef struct Table {
    TableRows rows;

    struct Table *parent;
    NodeList children;
} Table;


extern Table* table_create(Table *parent);





extern Table* check_semantics(AstNode *root);


#endif // _SEMANTIC_H
