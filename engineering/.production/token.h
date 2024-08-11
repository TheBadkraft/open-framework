#ifndef _TOKEN_H
#ifndef __token_h_ver
#define __token_h_ver "001-a"
#endif
#define _TOKEN_H

#include <stddef.h>

#include "file.h"
#include "cstring.h"

struct token_t
{
    // enum
    // {
    //     unknown,
    //     program,
    //     function,
    //     identifier,
    //     statement,
    //     numeric,
    //     expr
    // } type;
    // int line;
    glyph *pBeg;
    size_t length;
    //    struct token *prev;
    struct token_t *next;
};

typedef struct document_t
{
    file *pFile;
    string *pContent;
} document;

//  non-specialized token
typedef struct token_t token;

extern const struct Document_T
{
    document *(*loadp)(char *);
    document *(*loadf)(file *);
} Document;

extern const struct Token_T
{
    // Create a new token from begin and length
    token *(*new)(char *, size_t);
    char *(*word)(token *);
} Token;
#endif //  _TOKEN_H