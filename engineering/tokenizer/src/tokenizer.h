#ifndef _TOKENIZER_H
#ifndef __tokenizer_build
#define __tokenizer_build "0003"
#endif
#define _TOKENIZER_H

#include <stdio.h>

#include "token.h"

typedef token *(*tokenize)(document *);

typedef struct op_tokenizer
{
    const char *name;
    tokenize tokenize;
} tokenizer;

extern const struct OP_Tokenizer
{
    tokenizer *(*init)();
    tokenize tokenize;
} Tokenizer;
#endif //  _TOKENIZER_H
