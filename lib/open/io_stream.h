#ifndef _IO_STREAM_H
#define _IO_STREAM_H

#include "io.h"

enum io_mode
{
    BINARY = 1,
    READ = 2,
    WRITE = 4,
    APPEND = 8,
    CREATE = 16,
};

enum io_error
{
    NONE = 0,
    FILE_NOT_FOUND = 1,
    BAD_MODE = 2,
};

struct io_stream
{
    enum
    {
        OK = 0,
        ERR = 1
    } status;
    enum io_error error;
    enum io_mode mode;
    file *source;
    size_t length;
    size_t pos;
    FILE *fstream;
};

char *get_mode_label(enum io_mode);

// typedef enum io_mode IOMode;
typedef struct io_stream stream;

extern const struct IO_Stream
{
    bool (*new)(file *, enum io_mode, stream **);
    bool (*open)(stream *, enum io_mode);
    bool (*read)(stream *, char *);
    void (*free)(stream *);
    void (*get_error)(stream *, string *);
    char *(*err_info)(enum io_error);
    char *(*mode_info)(enum io_mode);
} Stream;

#endif //  _IO_STREAM_H