#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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
    uintptr_t uptr;
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
    bool (*dealloc)(void *);
    void (*flush)();
    void (*terminate)();
} Allocator;

#endif //  _ALLOCATOR_H