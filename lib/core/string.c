
#include <string.h>
#include <stdarg.h>

#include "open/core.h"

//  prototypes
bool __str_empty(string);
bool __str_null_or_empty(string);
void __str_new(size_t, string *);
void __str_copy(string, const string);
string __str_sub_copy(string, int, int);
void __str_free(string);
size_t __get_str_len(string);
size_t __get_str_cap(string);
void __str_append(string, const string);
void __str_format(const string, string *, ...);
void __str_trunc(string, size_t);
void __str_join(string, string *, ...);
string *__str_split(char, string);
int __fmt_count(const string);
//  -------------------

/*
    Determine if a string is empty; returns FALSE for NULL
*/
bool __str_empty(string pStr)
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
bool __str_null_or_empty(string pStr)
{
    if (!pStr || strcmp(String.empty, pStr) == 0)
    {
        return true;
    }

    return false;
}
/*
    Initializes a new '\0'-filled string to the specified length
*/
void __str_new(size_t capacity, string *out)
{
    (*out) = calloc(capacity, sizeof(char));
}
/*
    Allocates a new string from a copy of the given source
*/
void __str_alloc(string source, string *out)
{
    __str_new(strlen(source), out);
    strcpy((*out), source);
}
/*
    Copy string text to destination
*/
void __str_copy(string dest, const string text)
{
    // __set_str_cap(dest, strlen(text) + 1);
    strcpy(dest, text);
}
/*
    Copy substring to new string
*/
string __str_sub_copy(string text, int pos, int len)
{
    string pNew;
    __str_new(len, &pNew);

    strncpy(pNew, text + pos, len);

    return pNew;
}
/*
    Determines whether the current string is freeable
*/
bool __str_freeable(string text)
{
    // bool retOk = Core.is_allocated(text);

    return true;
}
/*
    Free the current string
*/
void __str_free(string text)
{
    // printf("free(str)\n");
    if (text != NULL)
    {
        free(text);
    }
}
/*
    Returns the string length
*/
size_t __get_str_len(string pStr)
{
    return __str_null_or_empty(pStr) ? 0 : strlen(pStr);
}
/*
    Appends the supplied string at the end of the current string.
*/
void __str_append(string base, const string text)
{
    size_t baselen = strlen(base), txtlen = strlen(text);
    size_t len = baselen + txtlen + 1;
    base = realloc(base, len);

    strcat(base, text);
}

/*
   // Appends a formatted string to the current string

void __str_appendf(string dest, const string format, ...)
{
  va_list args;

  va_start(args, format);
  int len = vsnprintf(NULL, 0, format, args);
  va_end(args);

  string pfStr;
  __str_new(len, &pfStr);

  va_start(args, format);
  vsprintf(pfStr, format, args);
  va_end(args);

  String.append(dest, pfStr);
  free(pfStr);
}
*/

/*
    Formats the given parameters returning a new string
*/
void __str_format(const string format, string *out, ...)
{
    va_list args;

    va_start(args, out);
    int len = vsnprintf(NULL, 0, format, args);
    va_end(args);

    __str_new(len, out);

    va_start(args, out);
    vsprintf((*out), format, args);
    va_end(args);
}
/*
    Truncate the current string
*/
void __str_trunc(string pStr, size_t len)
{
    if (__get_str_len(pStr) > len)
    {
        memset(pStr + len, 0, 1);
    }
}
/*
    Join multiple strings separated with the given delimeter.
*/
void __str_join(string delim, string *dest, ...)
{
    va_list args;

    //  first calculate how large the destination string needs to be
    va_start(args, dest);
    //  we play naive and only expect string (char*) args
    string arg = va_arg(args, string);
    size_t len = 1; //  set to 1 to account for null terminator
    while (arg)
    {
        len += strlen(arg);
        arg = va_arg(args, string);

        if (arg)
        {
            // __str_append(*dest, delim);
            len += strlen(delim);
        }
    }
    va_end(args);

    //  allocate the size one time
    __str_new(len, dest);

    //  iterate the args and join
    va_start(args, dest);
    arg = va_arg(args, string);
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
    va_end(args);
}
/*
    Split a string on a given delimter.
*/
string *__str_split(char delim, string pStr)
{
    //  not yet implemented
    return (char *[]){NULL};
}
/*
    Count the number of placeholders in a string format
*/
int __fmt_count(const string format)
{
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
    .is_empty = &__str_empty,
    .is_null_or_empty = &__str_null_or_empty,
    .new = &__str_new,
    .alloc = &__str_alloc,
    .copy = &__str_copy,
    .freeable = &__str_freeable,
    .free = &__str_free,
    .length = &__get_str_len,
    .append = &__str_append,
    // .appendf = &__str_appendf,
    .format = &__str_format,
    .truncate = &__str_trunc,
    .join = &__str_join,
    .split = &__str_split};
