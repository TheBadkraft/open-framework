#ifndef _OPEN_STRING_H
#define _OPEN_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

extern const struct IString
{
    //  empty string
    string const empty;
    //  Determine if a string is empty; returns FALSE for NULL
    bool (*is_empty)(string);
    //  Determine if a string is null or empty
    bool (*is_null_or_empty)(string);
    //  Initializes a new string to specified length
    void (*new)(size_t, string *);
    //  Allocates a new string from a copy of the given source
    void (*alloc)(string, string *);
    //  Resize the current string to the new size
    void (*resize)(size_t, string *);
    //  Copy string text to destination
    void (*copy)(string, const string);
    //  Begin copy source to destination at position
    void (*copy_to)(string, string, int loc);
    //  Copy substring to new string
    string (*subcopy)(string, int, int);
    //  [NOT WORKING] Determines whether the current string is freeable
    bool (*freeable)(string);
    //  Free the current string
    void (*dispose)(string);
    //  Returns the string length
    size_t (*length)(string);
    //  Appends the supplied string at the end of thecurrent string
    void (*append)(string, const string);

    /*
        //  Appends a formatted string to the current string
        void (*appendf)(string, const string, ...);
     */

    //  Formats the given parameters returning a new string
    void (*format)(const string, string *, ...);
    //  Truncate the current string
    void (*truncate)(size_t, string *);
    //  Join multiple strings separated with the given delimeter.
    void (*join)(string, string *, ...);
    //  determines the size of the joined string with delimeter
    size_t (*join_len)(string, ...);
    //  Split a string on a given delimter.
    string *(*split)(char, string);
} String;

#endif //  _SIGC_STRING_H
