#ifndef _READER_H
#define _READER_H

#include "file.h"

/*
    source file loader
*/
extern const struct Loader_T
{
    void (*init)();
    srcdoc *(*load)(file *);
    void (*destroy)();
} Loader;

#endif //  _READER_H