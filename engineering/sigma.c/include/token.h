#ifndef _TOKEN_H
#define _TOKEN_H

#include "sigctypes.h"

typedef struct token_t
{
    int line;
    glyph *pGlBeg;
    glyph *pGlEnd;
} token;

#endif //  _TOKEN_H