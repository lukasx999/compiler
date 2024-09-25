#pragma once
#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "lexer.h"
#include "parser.h"

#include <stdint.h>
#include <stdbool.h>


enum Datatype {
    DATATYPE_INTEGER,
    DATATYPE_FLOAT,
    DATATYPE_STRING,
    DATATYPE_BOOL,
    DATATYPE_VOID,
};


// Symbol table tree

typedef struct {
    char *identifier;
    char *value;
    enum Datatype type;

    // Token token;
} TableColumn; // column of the symbol table


typedef struct {
    size_t capacity;
    size_t size;
    TableColumn **nodes;
} TableColumnList;

// TODO: generic dynamic array




typedef struct Table Table;

// Dynamic Array of pointers to SymbolTables
typedef struct {
    size_t capacity;
    size_t size;
    Table **nodes;
} TableList;

extern void stnodeslist_init  (TableList *l);
extern void stnodeslist_append(TableList *l, Table *node);

struct Table {
    struct Table *parent; // NULL if root
    TableColumnList columns;
    TableList children;
};








extern void check_semantics(AstNode *root);


#endif // _SEMANTIC_H
