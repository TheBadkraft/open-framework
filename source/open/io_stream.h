#ifndef _IO_STREAM_H
#define _IO_STREAM_H

#include "io.h"

struct io_stream
{
    size_t length;
    size_t pos;
    char *source;
    bool is_open;
    FILE *fstream;
};

typedef struct io_stream stream;

#endif //  _IO_STREAM_H