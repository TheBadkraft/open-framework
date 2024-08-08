#ifndef _LEXER_H
#define _LEXER_H

#include "codex.h"
#include "file.h"

typedef struct lexer_t
{

} lexer;

extern const struct Lexer_T
{
    void (*tokenize)(srcdoc *);
} Lexer;

#endif //  _LEXER_H