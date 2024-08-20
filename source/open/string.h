#ifndef _OPEN_STRING_H
#define _OPEN_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

struct open_string
{
    char *buffer;
    size_t capacity;
};

typedef struct open_string string;

extern const struct Open_String
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
    void (*appendf)(string *, const char *, ...);
    string *(*format)(const char *, ...);
    void (*truncate)(string *, size_t);
    void (*writeln)(string *);
    string *(*join)(char *, ...);
    string *(*split)(char, string *);
} String;

#endif //  _SIGC_STRING_H
