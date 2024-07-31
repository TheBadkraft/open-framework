#ifndef REGISTRY_H
#define REGISTRY_H

#include <stdio.h>

#include "op/data_types.h"

#include "factory.h"

const int REG_LEN = 7;

typedef struct
{
    //  data
    int count;
    void *registry;
    Factory *factory;
} Registry;

typedef void (*RegisterFunc)(Registry *self, void *pRegistryItem);

#define REGISTER(_rFactory_)

#endif //  REGISTRY_H