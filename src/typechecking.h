#pragma once
#ifndef _TYPECHECKING_H
#define _TYPECHECKING_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "lexer.h"
#include "parser.h"


extern void check_types(AstNode *root);


#endif // _TYPECHECKING_H
