#pragma once
#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include "parser.h"







typedef struct {
    Token    identifier,
             datatype;
    uint64_t scope_level,
             rbp_offset;
} SymbolTableEntry;

/*  TODO:
 *
 * hash table of symboltables - lookup via string (global symbol table = NULL)
 * code generation: scope level member in struct: inc when entering, dec when leaving
 * -> use current stack frame string (or NULL for global) to lookup symbol table
 * -> find entry using linear (for now) search. search for identifier and scope level
 *
 */

extern void construct_symboltable(AstNode *root);


#endif // _SYMBOLTABLE_H
