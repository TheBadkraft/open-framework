#ifndef _FILE_H
#ifndef __file_h_ver
#define __file_h_ver "002-a"
#endif
#define _FILE_H

#include <stdio.h>

#include "sigctypes.h"

typedef struct file_t
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
} file;

typedef struct stream_t
{
    size_t length;
    size_t pos;
    char *source;
    bool is_open;
    FILE *fstream;
} stream;

extern const struct File_T
{
    bool (*exists)(char *);
    size_t (*size)(char *);
    file *(*new)(char *);
    stream *(*open)(file *);
    void (*close)(stream *);
} File;

extern const struct Stream_T
{
    bool (*read)(stream *, char *);
} Stream;

#endif //  _FILE_H
