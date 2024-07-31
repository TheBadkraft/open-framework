#ifndef OBJECT_H
#define OBJECT_H

#include <uuid/uuid.h>

struct Object
{
    /* data */
    char *guid;
};

extern const struct Class
{
    struct Object (*new)();
} Object;

#endif //  OBJECT_H