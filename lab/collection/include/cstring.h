#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct string_t;

typedef struct string_t
{
    char *buffer;
    size_t capacity;
} string;

extern const struct String_T
{
    string *(*new)();
    string *(*alloc)(const char *text);
    void (*copy)(string *pStr, const char *text);
    void (*free)(string *pStr);
    size_t (*length)(string *pStr);
    void (*capacity)(string *pStr, size_t cap);
    void (*append)(string *pStr, const char *text);
    string *(*format)(const char *format, ...);
    void (*truncate)(string *pStr, size_t len);
} String;

//  prototypes
static string *new();
static string *alloc_str(const char *text);
static void copy_str(string *pStr, const char *text);
static void free_str(string *pStr);
static size_t get_length(string *pStr);
static void set_capacity(string *pStr, size_t len);
static void append_str(string *pStr, const char *text);
static string *format_str(const char *format, ...);
static void truncate_str(string *pStr, size_t len);
//  -------------------

#endif //  STRING_H