#ifndef OPIO_H
#define OPIO_H

#include <unistd.h>

#include "cstring.h"

static const int PATH_MAX = 255;

typedef struct path_t
{
    string *path;
} path;

typedef struct dir_t
{
    path *current;
} directory;

static string *_get_current_directory()
{
    char cwd[PATH_MAX];
    return String.alloc(getcwd(cwd, PATH_MAX));
}

#define CURRENT_DIR _get_current_directory()

#endif //  OPIO_H