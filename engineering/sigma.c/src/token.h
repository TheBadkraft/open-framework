#ifndef _TOKEN_H
#define _TOKEN_H

#include "sigctypes.h"
#include "codex.h"

struct token {
    int line;
    glyph *pBeg;
    size_t length;
    enum LexType type;
//    struct token *prev;
    struct token *next;
};

extern const struct Token_T {
	struct token *(*new)(char*, size_t);
	char *(*word)(struct token*);
} Token;
#endif //  _TOKEN_H
