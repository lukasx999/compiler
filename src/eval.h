#pragma once
#ifndef _EVAL_H
#define _EVAL_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lexer.h"
#include "parser.h"


enum Datatype_kind {
    KIND_INTEGER,
    KIND_STRING,
    KIND_BOOLEAN,
    KIND_NIL,
};


typedef struct {

    enum Datatype_kind kind;

    union {
        double d_int; // for simplicities sake, this language will only support floating point numbers => we'll treat ints and floats the same, except that ints are syntactic sugar for floats, eg: 3 = 3.0
        char d_string[BUFSIZE];
        bool d_bool;
        bool d_nil; // is true if value is nil
    };

} Datatype;



extern Datatype evaluate(AstNode *root);




#endif // _EVAL_H
