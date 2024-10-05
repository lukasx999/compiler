#pragma once
#ifndef _CODEGEN_H
#define _CODEGEN_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "parser.h"

#define ASM_FILENAME "out.asm"
#define OBJ_FILENAME "out.o"
#define BIN_FILENAME "out"


typedef struct {
    vec_Vector text;
    vec_Vector data;
    // vec_Vector bss;
    // vec_Vector rodata;
} Sections; // Assembly code sections


extern void compile(AstNode *root);





#endif // _CODEGEN_H
