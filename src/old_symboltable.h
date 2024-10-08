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


static size_t datatype_size[] = {
    [DATATYPE_INTEGER]  = 4,
    [DATATYPE_FLOAT]    = 8,
    [DATATYPE_STRING]   = 8,
    [DATATYPE_BOOL]     = 1,
    [DATATYPE_VOID]     = 0,
    [DATATYPE_FUNCTION] = 8,
};


static char datatype_repr[][BUFSIZE] = {
    [DATATYPE_INTEGER]  = "int",
    [DATATYPE_FLOAT]    = "float",
    [DATATYPE_STRING]   = "string",
    [DATATYPE_BOOL]     = "bool",
    [DATATYPE_VOID]     = "void",
    [DATATYPE_FUNCTION] = "func",
};







// Symbol table tree

typedef struct {
    char *identifier; // name
    enum Datatype type;
    size_t address; // memory location (offset to rbp)
    // scope

    bool not_initialized;

    // Token token;
} TableColumn; // column of the symbol table

typedef vec_Vector TableRows; // array of columns
typedef vec_Vector NodeList;  // array of pointers to several child nodes

typedef struct Table {
    TableRows rows; // list of columns
    struct Table *parent;
    NodeList children;
    bool stack_frame;
} Table;


extern Table* table_create(Table *parent);





extern Table* construct_symboltable(AstNode *root);


#endif // _SEMANTIC_H
