#ifndef _CURSOR_H
#define _CURSOR_H

#include "../core/types.h"
#include "../core/allocator.h"

struct io_cursor
{
    char *pPos;
    size_t length;
    struct io_cursor *next;
};

//  non-specialized token
typedef struct io_cursor *cursor;

extern const struct ICursor
{
    // Create a new token from begin and length
    cursor (*new)(char *, size_t);
    bool (*word)(cursor, string *);
    void (*dispose)(cursor);
    bool (*equals)(cursor, string);
} Cursor;
#endif //  _CURSOR_H
