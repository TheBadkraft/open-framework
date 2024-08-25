
#include <string.h>
#include <stdarg.h>

#include "../open/string.h"

//  prototypes
string *__str_new();
string *__str_alloc(const char *);
void __str_copy(string *, const char *);
string *__str_sub_copy(string *, int, int);
void __str_free(string *);
size_t __get_str_len(string *);
void __set_str_cap(string *, size_t);
void __str_append(string *, const char *);
string *__str_format(const char *, ...);
void __str_trunc(string *, size_t);
size_t __fmt_count(const char *);
//  -------------------

bool str_null_or_empty(string *pStr)
{
    if (!pStr || strcmp(String.empty, pStr->buffer) == 0)
    {
        return true;
    }
}
string *__str_new()
{
    string *pStr = malloc(sizeof(string));
    pStr->buffer = NULL;
    pStr->capacity = 0;

    return pStr;
}
string *__str_alloc(const char *text)
{
    string *pStr = __str_new();
    // printf("size: %ld\n", pStr->capacity);

    __str_copy(pStr, text);

    // printf("set string capacity: %ld (%ld)\n", pStr->capacity, strlen(pStr->buffer));

    return pStr;
}
void __str_copy(string *pStr, const char *text)
{
    __set_str_cap(pStr, strlen(text) + 1);
    strcpy(pStr->buffer, text);
}
string *__str_sub_copy(string *pStr, int pos, int len)
{
    string *pNew = String.new();
    __set_str_cap(pNew, len + 1);
    strncpy(pNew->buffer, pStr->buffer + pos, len);

    return pNew;
}
void __str_free(string *pStr)
{
    // printf("free(str)\n");
    if (pStr != NULL)
    {
        if (pStr->buffer != NULL)
        {
            free(pStr->buffer);
        }
        free(pStr);
    }
}
size_t __get_str_len(string *pStr)
{
    return (pStr->buffer == NULL) ? 0 : strlen(pStr->buffer);
}
void __set_str_cap(string *pStr, size_t cap)
{
    if (pStr->buffer == NULL)
    {
        pStr->buffer = malloc(cap);
    }
    else if (__get_str_len(pStr) != cap)
    {
        pStr->buffer = realloc(pStr->buffer, cap);
    }

    pStr->capacity = cap;
}
void __str_append(string *pStr, const char *text)
{
    __set_str_cap(pStr, pStr->capacity + strlen(text) + 1);
    strcat(pStr->buffer, text);
}
string *__str_format(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    int capacity = vsnprintf(NULL, 0, format, args);
    va_end(args);

    string *pStr = __str_new();
    __set_str_cap(pStr, capacity + 1);

    va_start(args, format);
    vsprintf(pStr->buffer, format, args);
    va_end(args);

    return pStr;
}
void __str_appendf(string *pStr, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    int capacity = vsnprintf(NULL, 0, format, args);
    va_end(args);

    char *pBuff = malloc(capacity + 1);

    va_start(args, format);
    vsprintf(pBuff, format, args);
    va_end(args);

    String.append(pStr, pBuff);
    free(pBuff);
}
void __str_trunc(string *pStr, size_t len)
{
    if (__get_str_len(pStr) > len)
    {
        memset(pStr->buffer + len, 0, 1);
    }
}
void __str_write_ln(string *pStr)
{
    printf("%s\n", pStr->buffer != NULL ? pStr->buffer : "NULL");
}
string *__str_join(char *delim, ...)
{
    //  not yet implemented
    return String.new();
}
string *__str_split(char delim, string *pStr)
{
    //  not yet implemented
    return String.new();
}
size_t __fmt_count(const char *format)
{
    size_t count = 0;
    while (*format)
    {
        if (*format++ == '%')
        {
            ++count;
        }
    }

    return count;
}

const struct Open_String String = {
    .empty = "",
    .is_null_or_empty = &str_null_or_empty,
    .new = &__str_new,
    .alloc = &__str_alloc,
    .copy = &__str_copy,
    .release = &__str_free,
    .free = &__str_free,
    .length = &__get_str_len,
    .capacity = &__set_str_cap,
    .append = &__str_append,
    .appendf = &__str_appendf,
    .format = &__str_format,
    .truncate = &__str_trunc,
    .writeln = &__str_write_ln,
    .join = &__str_join,
    .split = &__str_split};
