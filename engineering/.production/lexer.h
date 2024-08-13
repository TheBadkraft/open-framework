#ifndef _LEXER_H
#ifndef __lexer_build
#define __lexer_build "0002"
#endif
#define _LEXER_H

#include <stdio.h>

#include "token.h"

typedef token *(*tokenizer)(document *);

typedef struct lexer_t
{
    const char *name;
    tokenizer tokenize;
} lexer;

extern const struct Lexer_T
{
    lexer *(*init)();
    tokenizer tokenize;
} Lexer;
#endif //  _LEXER_H