#ifndef _INDEXER_H
#define _INDEXER_H

#include <stdio.h>

#include "../core/string.h"

#include "io.h"
#include "token.h"

struct io_indexer;
struct io_document
{
    string source;
    string content;
};

typedef struct io_document *document;
typedef bool (*index_delegate)(document, token *);

struct io_indexer
{
    string name;
    index_delegate tokenize;
};

typedef struct io_indexer *indexer;

//  generic indexer functions
static const char SPACE = ' ';
static const char NEWLINE = '\n';

static bool is(char c, char cmp)
{
    return c == cmp;
}
static bool is_any(char c, string cmps)
{
    //  to use this properly, terminate series with '\0'
    bool retAny = false;
    char *ndx = cmps;
    while (*ndx != '\0')
    {
        retAny = is(c, *ndx);

        if (retAny)
        {
            break;
        }

        ++ndx;
    }

    return retAny;
}
static bool is_noise(char c)
{
    return is(c, SPACE) || is(c, NEWLINE);
}
static char *get_word(char *pChar, string delims, size_t strLen)
{
    if (delims == NULL)
    {
        delims = " \0";
    }
    char *ndx = pChar;
    char *end;

    while (ndx - pChar < strLen)
    {
        if (is_any(*ndx, delims))
        {
            end = ndx - 1;

            break;
        }
        ++ndx;
    }

    return end;
}

//  interfaces
extern const struct Document_T
{
    document (*new)(void);
    bool (*load)(stream, document *);
    bool (*dispose)(document);
} Document;

extern const struct IO_Indexer
{
    bool (*init)(string, index_delegate, indexer *);
    bool (*tokenize)(document, token *);
    //  TODO: !!!
    void (*dispose)(indexer);
} Indexer;

#endif //  _INDEXER_H
