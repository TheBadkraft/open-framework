#ifndef STRING_H
#ifndef __cstring_h_ver
#define __cstring_h_ver "002-a"
#endif
#define STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sigctypes.h"

typedef struct string_t
{
    char *buffer;
    size_t capacity;
} string;

extern const struct String_T
{
    const char *empty;
    bool (*is_null_or_empty)(string *);
    string *(*new)();
    string *(*alloc)(const char *);
    void (*copy)(string *, const char *);
    void (*free)(string *);
    size_t (*length)(string *);
    void (*capacity)(string *, size_t);
    void (*append)(string *, const char *);
    string *(*format)(const char *, ...);
    void (*truncate)(string *, size_t);
} String;

#endif //  STRING_H
