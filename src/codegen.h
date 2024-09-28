#pragma once
#ifndef _CODEGEN_H
#define _CODEGEN_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "parser.h"


extern void compile(AstNode *root);



#endif // _CODEGEN_H
