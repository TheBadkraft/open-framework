#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

#include <stddef.h>
#include <stdbool.h>

#include "types.h"

#if DEBUG
#include "../open/test.h"
#endif

enum Alloc_Mode
{
    INITIALIZED,
    UNINITIALIZED
};

struct Mem_Block
{
    objptr obj_ptr;
};

struct Mem_Page
{
    bool is_initialized;
    struct Mem_Block *page;
    size_t count;
    size_t capacity;
};

typedef enum Alloc_Mode allocMode;
typedef struct Mem_Block *mem_block;
typedef struct Mem_Page *mem_page;

extern const struct Open_Allocator
{
    size_t (*count)();
    size_t (*capacity)();
    void *(*alloc)(size_t, enum Alloc_Mode);
    bool (*dealloc)(object);
    void (*flush)();
    void (*terminate)();
    bool (*is_alloc)(object);
} Allocator;

#endif //  _ALLOCATOR_H