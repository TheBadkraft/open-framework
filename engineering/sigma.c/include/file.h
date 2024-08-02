#ifndef _FILE_H
#define _FILE_H

#include <unistd.h>

#include "../include/sigctypes.h"

typedef struct file_t
{
    char *name;
    bool exists;
} file;

file *file_exists(char *);

#endif //  _FILE_H