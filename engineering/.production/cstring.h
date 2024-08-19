#ifndef _CSTRING_H
#ifndef __cstring_build
#define __cstring_build 5
#endif
#define _CSTRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sigC/sigma_types.h"

typedef struct sigc_string
{
    char *buffer;
    size_t capacity;
};

typedef struct sigc_string string;

//  prototypes
string *new_str();
string *alloc_str(const char *text);
void copy_str(string *pStr, const char *text);
void free_str(string *pStr);
size_t get_length(string *pStr);
void set_capacity(string *pStr, size_t len);
void append_str(string *pStr, const char *text);
string *format_str(const char *format, ...);
void truncate_str(string *pStr, size_t len);
//  -------------------

const struct String_T
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

#endif //  _CSTRING_H
