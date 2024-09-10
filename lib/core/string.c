
#include <string.h>
#include <stdarg.h>

#include "open/core.h"
#include "open/allocator.h"
#include "open/internal/internal_string.h"

//  String expects Allocator to be initialized.

//  prototypes
bool str_empty(string);
bool str_null_or_empty(string);
void str_new(size_t, string *);
void str_alloc(string, string *);
void str_copy(string, const string);
void str_copy_to(string, string, int);
string str_sub_copy(string, int, int);
void str_free(string);
size_t get_str_len(string);
size_t get_str_cap(string);
void str_append(string, const string);
void str_format(const string, string *, ...);
void str_trunc(size_t, string *);
void str_join(string, string *, ...);
string *str_split(char, string);
int fmt_count(const string);
//  -------------------

/*
    Determine if a string is empty; returns FALSE for NULL
*/
bool str_empty(string pStr)
{
    if (pStr == NULL)
    {
        return false;
    }
    else
    {
        return strcmp(String.empty, pStr) == 0;
    }
}
/*
    Determine if a string is null or empty
*/
bool str_null_or_empty(string pStr)
{
    if (!pStr || strcmp(String.empty, pStr) == 0)
    {
        return true;
    }

    return false;
}
/*
    Initializes a new '\0'-filled string to the specified length; adds 1 for string terminator
*/
void str_new(size_t capacity, string *out)
{
    (*out) = Allocator.alloc(capacity + 1, INITIALIZED);
}
/*
    Allocates a new string from a copy of the given source
*/
void str_alloc(string source, string *out)
{
    str_new(strlen(source), out);
    strcpy((*out), source);
}

void str_resize(size_t size, string *out)
{
    size_t len = get_str_len(*out);
    if (len == size)
    {
        return;
    }
    if (len > size)
    {
        str_trunc(size, out);
    }
    else
    {
        (*out) = realloc(*out, size);
    }
}
/*
    Copy string text to destination
*/
void str_copy(string dest, const string text)
{
    // __set_str_cap(dest, strlen(text) + 1);
    strcpy(dest, text);
}
/*
    Begin copy source to destination at position
*/
void str_copy_to(string text, string dest, int loc)
{
    strcpy((dest + loc), text);
}
/*
    Copy substring to new string
*/
string str_sub_copy(string text, int pos, int len)
{
    string pNew;
    str_new(len, &pNew);

    strncpy(pNew, text + pos, len);

    return pNew;
}
/*
    Determines whether the current string is freeable
*/
bool str_freeable(string text)
{
    // bool retOk = Core.is_allocated(text);

    return true;
}
/*
    Free the current string
*/
void str_free(string text)
{
    // printf("free(str)\n");
    if (text != NULL)
    {
        Allocator.dealloc(text);
    }
}
/*
    Returns the string length
*/
size_t get_str_len(string pStr)
{
    return str_null_or_empty(pStr) ? 0 : strlen(pStr);
}
/*
    Appends the supplied string at the end of the current string.
*/
void str_append(string base, const string text)
{
    size_t baselen = strlen(base), txtlen = strlen(text);
    size_t len = baselen + txtlen + 1;
    base = realloc(base, len);

    strcat(base, text);
}
/*
    Formats the given parameters returning a new string
*/
void str_format(const string format, string *out, ...)
{
    va_list args;

    va_start(args, out);
    int len = vsnprintf(NULL, 0, format, args);
    va_end(args);

    str_new(len, out);

    va_start(args, out);
    vsprintf((*out), format, args);
    va_end(args);
}
/*
    Truncate the current string
*/
void str_trunc(size_t size, string *out)
{
    // we allocate for the null terminator here
    if (get_str_len(*out) > size)
    {
        (*out) = realloc(*out, size + 1);
        memset((*out) + size, 0, 1);
    }
}
/*
    Join multiple strings separated with the given delimeter. Expect dest to be malloc'd and
    properly sized.
*/
void str_join(string delim, string *dest, ...)
{
    va_list args;

    va_start(args, dest);
    str_vjoin(delim, dest, args);
    va_end(args);
}
size_t str_join_len(string delim, ...)
{
    va_list args;
    va_start(args, delim);
    string dest = String.empty;
    size_t size = str_vsnjoin(delim, &dest, args);
    va_end(args);

    return size;
}
/*
    Split a string on a given delimter.
*/
string *str_split(char delim, string pStr)
{
    //  not yet implemented
    return NULL;
}

//  INTERNAL FUNCTIONS

/*
    In this internal implementation, we expect that 'dest' is malloc'd, and of the proper
    length.
*/
void str_vjoin(string delim, string *dest, va_list args)
{
    if (*dest == NULL)
    {
        perror("destination for join cannot be NULL; must be allocated & sized");
        return;
    }

    if (!str_empty(*dest))
    {
        strcat(*dest, delim);
    }
    //  iterate the args and join
    string arg = va_arg(args, string);
    while (arg)
    {
        strcat(*dest, arg);
        //  get the next arg
        arg = va_arg(args, string);
        if (arg)
        {
            strcat(*dest, delim);
        }
    }
}
/*
    Expect 'dest' to NOT be NULL. Count the final string length of a join with delimeter
*/
size_t str_vsnjoin(string delim, string *dest, va_list args)
{
    size_t delimLen = String.length(delim);
    size_t outLen = String.length(*dest);

    string arg = va_arg(args, string);
    while (arg)
    {
        outLen += delimLen + String.length(arg);
        arg = va_arg(args, string);
    }

    return outLen;
}

//  MISCELLANEOUS
/*
    Count the number of placeholders in a string format

    NAIVE
*/
int fmt_count(const string format)
{
    /*
        NOT USED in any functions.
        ASSUMPTION: any '%' is a format placeholder.
    */
    char *p = format;
    int count = 0;
    while (p)
    {
        if (*p == '%')
        {
            ++count;
        }

        ++p;
    }

    return count;
}

const struct Open_String String = {
    .empty = "",
    .is_empty = &str_empty,
    .is_null_or_empty = &str_null_or_empty,
    .new = &str_new,
    .alloc = &str_alloc,
    .resize = &str_resize,
    .copy = &str_copy,
    .copy_to = &str_copy_to,
    .freeable = &str_freeable,
    .free = &str_free,
    .length = &get_str_len,
    .append = &str_append,
    .format = &str_format,
    .truncate = &str_trunc,
    .join = &str_join,
    .join_len = &str_join_len,
    .split = &str_split};
