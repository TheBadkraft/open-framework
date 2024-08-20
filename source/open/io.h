#ifndef _IO_H
#define _IO_H

#include "core.h"

enum io_type
{
    IO_NONE = -1,
    IO_UNKNOWN = 0,
    IO_FILE = 1,
    IO_DIRECTORY = 2
};

struct io_file
{
    enum
    {
        BINARY = 1,
        READ = 2,
        WRITE = 4,
        APPEND = 8,
        CREATE = 16
    } mode;
    char *name;
    size_t size;
    bool exists;
};

typedef enum io_type IOType;
typedef struct io_file file;

#endif //  _IO_H