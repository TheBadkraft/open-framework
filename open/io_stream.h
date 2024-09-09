#ifndef _IO_STREAM_H
#define _IO_STREAM_H

#include <stdio.h>

#include "types.h"
#include "string.h"
#include "allocator.h"

enum io_mode
{
    NO_MODE = -1,
    BINARY = 1,
    READ = 2,
    WRITE = 4,
    APPEND = 8,
    CREATE = 16,
};

enum io_error
{
    NONE = 0,
    PATH_NOT_FOUND = 1,
    BAD_MODE = 2,
};

struct io_stream
{
    enum
    {
        ERR = 1,
        OK = 2,
        OPEN = 4,
        CLOSED = 8,
    } status;
    string source;
    enum io_error error;
    enum io_mode mode;
    size_t length;
    size_t pos;
    FILE *fstream;
};

typedef struct io_stream *stream;

extern const struct Open_Stream
{
    stream (*new)(string);
    void (*get_error)(stream, string *);
    void (*err_info)(enum io_error, string *);
    void (*mode_info)(enum io_mode, string *);
    bool (*is_open)(stream);
    void (*free)(stream);
    void (*mode_to_string)(stream);
} Stream;

#endif //  _IO_STREAM_H