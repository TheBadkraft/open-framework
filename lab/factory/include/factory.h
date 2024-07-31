#ifndef FACTORY_H
#define FACTORY_H

#include "op/data_types.h"

#include "registry.h"

typedef struct
{
    /* data */
    const CHAR *name;
    void *build;
} Factory;

#define CREATE_FACTORY(_fName_, _makefunc_) Factory _fName_##Factory = {#_fName_, _makefunc_}

#endif //  FACTORY_H