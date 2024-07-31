#ifndef _CLASS_H
#define _CLASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cstring.h"
#include "reftable.h"

typedef struct
{
    rt_entry *ref;
    string *to_string;
} class_t;

typedef class_t *(*cls_new)(const char *type_name, void *t);

extern const struct Class_t
{
    //  new class reference object
    cls_new new;
} Class;

#define BEGIN_CLASS(_type)   \
    typedef struct _type##_t \
    {                        \
        class_t *__class;

// #define END_CLASS(_type) \
//     }                    \
//     _type;               \
//     void init_##_type(_type *t);
#define END_CLASS(_type) \
    }                    \
    _type;

#define INIT_CLASS(_type)                  \
    void init_##_type(_type *t)            \
    {                                      \
        t->__class = Class.new(#_type, t); \
    }

#endif //  _CLASS_H