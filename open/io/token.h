#ifndef _TOKEN_H
#define _TOKEN_H

#include "../core/types.h"
#include "../core/allocator.h"

struct io_token
{
    char *pPos;
    size_t length;
    struct io_token *next;
};

//  non-specialized token
typedef struct io_token *token;

extern const struct Token_T
{
    // Create a new token from begin and length
    token (*new)(char *, size_t);
    bool (*word)(token, string *);
    void (*free)(token);
} Token;
#endif //  _TOKEN_H
