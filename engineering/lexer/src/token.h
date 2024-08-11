#ifndef _TOKEN_H
#define _TOKEN_H

#include <stddef.h>

#include "sigctypes.h"
// #include "codex.h"

struct token
{
    enum
    {
        unknown,
        program,
        function,
        identifier,
        statement,
        numeric,
        expr
    } type;
    int line;
    glyph *pBeg;
    size_t length;
    //    struct token *prev;
    struct token *next;
};

extern const struct Token_T
{
    struct token *(*new)(char *, size_t);
    char *(*word)(struct token *);
} Token;
#endif //  _TOKEN_H
