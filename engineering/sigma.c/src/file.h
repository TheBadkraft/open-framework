#ifndef _FILE_H
#define _FILE_H

#include <unistd.h>

#include "sigctypes.h"
#include "token.h"

typedef struct file_t {
    char *name;
    size_t size;
    bool exists;
} file;

typedef struct srcdoc_t {
    file *pFile;
    char *source;
    struct token *pToken;
} srcdoc;

file *file_exists(char *);

#endif //  _FILE_H
