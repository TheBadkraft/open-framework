#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct Mem_Pointer
{
    uintptr_t addr;
    void *item;
};

struct Mem_Allocator
{
    struct Mem_Pointer *stack;
    size_t count;
    size_t capacity;
};

typedef struct Mem_Pointer *mem_ptr;
typedef struct Mem_Allocator *mem_allocator;

extern const struct Open_Allocator
{
    bool (*init)();
    size_t (*count)();
    size_t (*capacity)();
    void (*clear)();
    void (*terminate)();
    void *(*alloc)(size_t);
    bool (*dealloc)(void *);
    mem_ptr (*pointer)(uintptr_t);
} Allocator;

#endif //  _ALLOCATOR_H