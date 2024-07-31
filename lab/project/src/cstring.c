
#include <string.h>
#include <stdarg.h>

#include "../include/cstring.h"

static string *new()
{
    string *pStr = malloc(sizeof(string));
    pStr->buffer = NULL;
    pStr->capacity = 0;

    return pStr;
}
static string *alloc_str(const char *text)
{
    string *pStr = new ();
    // printf("size: %ld\n", pStr->capacity);

    copy_str(pStr, text);

    // printf("set string capacity: %ld (%ld)\n", pStr->capacity, strlen(pStr->buffer));

    return pStr;
}
static void copy_str(string *pStr, const char *text)
{
    set_capacity(pStr, strlen(text) + 1);
    strcpy(pStr->buffer, text);
}
static void free_str(string *pStr)
{
    if (pStr->buffer != NULL)
    {
        free(pStr->buffer);
    }
    if (pStr != NULL)
    {
        free(pStr);
    }
}
static size_t get_length(string *pStr)
{
    return (pStr->buffer == NULL) ? 0 : strlen(pStr->buffer);
}
static void set_capacity(string *pStr, size_t cap)
{
    if (pStr->buffer == NULL)
    {
        pStr->buffer = malloc(cap);
    }
    else if (get_length(pStr) != cap)
    {
        pStr->buffer = realloc(pStr->buffer, cap);
    }

    pStr->capacity = cap;
}
static void append_str(string *pStr, const char *text)
{
    set_capacity(pStr, pStr->capacity + strlen(text) + 1);
    strcat(pStr->buffer, text);
}
static string *format_str(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    int capacity = vsnprintf(NULL, 0, format, args);
    va_end(args);

    string *pStr = new ();
    set_capacity(pStr, capacity + 1);

    va_start(args, format);
    vsprintf(pStr->buffer, format, args);
    va_end(args);

    return pStr;
}
static void truncate_str(string *pStr, size_t len)
{
    if (get_length(pStr) > len)
    {
        memset(pStr->buffer + len, 0, 1);
    }
}

const struct String_T String = {
    .new = &new,
    .alloc = &alloc_str,
    .copy = &copy_str,
    .free = &free_str,
    .length = &get_length,
    .capacity = &set_capacity,
    .append = &append_str,
    .format = &format_str,
    .truncate = &truncate_str};