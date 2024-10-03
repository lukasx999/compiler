#pragma once
#ifndef _CODEGEN_H
#define _CODEGEN_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "parser.h"

#define ASM_FILENAME "out.s"
#define OBJ_FILENAME "out.o"
#define BIN_FILENAME "out"



extern void compile(AstNode *root);





#endif // _CODEGEN_H
