#pragma once
#ifndef _ERR_H
#define _ERR_H

#include "parser.h"



extern void err_correct(Token tok, const char *message, const char *correct, char *source, const char *filename);
extern void err_throw(Token tok, const char *message, char *source, const char *filename);


#endif // _ERR_H
