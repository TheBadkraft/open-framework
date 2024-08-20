
#include <string.h>
#include <stdarg.h>

#include "../open/string.h"

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
size_t parse_format(const char *);
//  -------------------

bool str_null_or_empty(string *pStr)
{
    if (!pStr || strcmp(String.empty, pStr->buffer) == 0)
    {
        return true;
    }
}
string *new_str()
{
    string *pStr = malloc(sizeof(string));
    pStr->buffer = NULL;
    pStr->capacity = 0;

    return pStr;
}
string *alloc_str(const char *text)
{
    string *pStr = new_str();
    // printf("size: %ld\n", pStr->capacity);

    copy_str(pStr, text);

    // printf("set string capacity: %ld (%ld)\n", pStr->capacity, strlen(pStr->buffer));

    return pStr;
}
void copy_str(string *pStr, const char *text)
{
    set_capacity(pStr, strlen(text) + 1);
    strcpy(pStr->buffer, text);
}
void free_str(string *pStr)
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
size_t get_length(string *pStr)
{
    return (pStr->buffer == NULL) ? 0 : strlen(pStr->buffer);
}
void set_capacity(string *pStr, size_t cap)
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
void append_str(string *pStr, const char *text)
{
    set_capacity(pStr, pStr->capacity + strlen(text) + 1);
    strcat(pStr->buffer, text);
}
size_t parse_format(const char *format)
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
string *format_str(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    int capacity = vsnprintf(NULL, 0, format, args);
    va_end(args);

    string *pStr = new_str();
    set_capacity(pStr, capacity + 1);

    va_start(args, format);
    vsprintf(pStr->buffer, format, args);
    va_end(args);

    return pStr;
}
void append_format_str(string *pStr, const char *format, ...)
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
void truncate_str(string *pStr, size_t len)
{
    if (get_length(pStr) > len)
    {
        memset(pStr->buffer + len, 0, 1);
    }
}
void write_line(string *pStr)
{
    printf("%s\n", pStr->buffer);
}
string *join_str(char *delim, ...)
{
    //  not yet implemented
    return String.new();
}
string *split_str(char delim, string *pStr)
{
    //  not yet implemented
    return String.new();
}

const struct Open_String String = {
    .empty = "",
    .is_null_or_empty = &str_null_or_empty,
    .new = &new_str,
    .alloc = &alloc_str,
    .copy = &copy_str,
    .free = &free_str,
    .length = &get_length,
    .capacity = &set_capacity,
    .append = &append_str,
    .appendf = &append_format_str,
    .format = &format_str,
    .truncate = &truncate_str,
    .writeln = &write_line,
    .join = &join_str,
    .split = &split_str};
