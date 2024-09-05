#include <stdio.h>
#include <stdlib.h>

#include "open/allocator.h"

static size_t DEFAULT_STACK_SIZE = 64;
static mem_allocator ALLOCATOR = NULL;
static struct Mem_Pointer NULL_POINTER = {
    .addr = 0,
    .item = NULL};

//  PROTOTYPES
void __init();
void __stack_add_ptr(void *);
bool __stack_rem_ptr(void *);
mem_ptr __stack_get_pointer(uintptr_t, int *);
void __stack_repl_pointer(mem_ptr, mem_ptr, int);
void __mem_ptr_free(mem_ptr);
bool __mem_ptr_equal(mem_ptr, mem_ptr);

bool __alloc_init();
size_t __alloc_count();
void __alloc_clear();
void __alloc_terminate();
void *__alloc_allocate(size_t);
bool __alloc_deallocate(void *);
mem_ptr __alloc_get_pointer(uintptr_t);

void __init()
{
    ALLOCATOR = malloc(sizeof(struct Mem_Allocator));
    ALLOCATOR->count = 0;
    ALLOCATOR->capacity = DEFAULT_STACK_SIZE;
    ALLOCATOR->stack = malloc(sizeof(struct Mem_Pointer) * ALLOCATOR->capacity);
}
void __stack_add_ptr(void *ptr)
{
    //  we can check for eligible empty locations
    int ndx;
    mem_ptr memPtr = __stack_get_pointer(0, &ndx);
    if (memPtr != NULL)
    {
        mem_ptr memPtr = malloc(sizeof(struct Mem_Pointer));
        memPtr->item = ptr;
        memPtr->addr = (uintptr_t)ptr;

        ALLOCATOR->stack[ndx] = *memPtr;
        ALLOCATOR->count++;
    }
    else
    {
        if (ALLOCATOR->count == ALLOCATOR->capacity)
        {
            // resize
            printf("ERROR: need to resize mem stack ...\n");
            return;
        }
        else
        {
            mem_ptr memPtr = malloc(sizeof(struct Mem_Pointer));
            memPtr->item = ptr;
            memPtr->addr = (uintptr_t)ptr;

            ALLOCATOR->stack[ALLOCATOR->count++] = *memPtr;
        }
    }
}
bool __stack_rem_ptr(void *ptr)
{
    int ndx;
    mem_ptr memPtr = __stack_get_pointer((uintptr_t)ptr, &ndx);

    if (ndx != -1)
    {
        __stack_repl_pointer(ALLOCATOR->stack + ndx, &NULL_POINTER, ndx);
        __mem_ptr_free(memPtr);
        ALLOCATOR->count--;
    }

    return ndx != -1;
}
mem_ptr __stack_get_pointer(uintptr_t addr, int *ndx)
{
    *ndx = 0;
    mem_ptr memPtr = NULL;

    while (*ndx < ALLOCATOR->capacity)
    {
        if (ALLOCATOR->stack[*ndx].addr == addr)
        {
            memPtr = ALLOCATOR->stack + *ndx;
            break;
        }
        *++ndx;
    }
    if (!memPtr)
    {
        (*ndx) = -1;
    }

    return memPtr;
}
void __stack_repl_pointer(mem_ptr targ, mem_ptr with, int ndx)
{
    ALLOCATOR->stack[ndx] = NULL_POINTER;
}
void __mem_ptr_free(mem_ptr memPtr)
{
    if (memPtr != NULL)
    {
        if (memPtr->item != NULL)
        {
            free(memPtr->item);
            memPtr->item = NULL;
        }

        free(memPtr);
        memPtr = NULL;
    }
}
bool __mem_ptr_equal(mem_ptr mp1, mem_ptr mp2)
{
    return mp1->addr == mp2->addr &&
           mp1->item == mp2->item;
}

bool __alloc_init()
{
    if (ALLOCATOR == NULL)
    {
        __init();
    }

    return ALLOCATOR != NULL;
}
size_t __alloc_count()
{
    return ALLOCATOR->count;
}
size_t __alloc_cap()
{
    return ALLOCATOR->capacity;
}
void __alloc_clear()
{

    //  for each memPtr: remove & free
    int ndx = 0;
    while (ndx < ALLOCATOR->capacity)
    {
        if (!__mem_ptr_equal(&NULL_POINTER, ALLOCATOR->stack + ndx))
        {
            __stack_repl_pointer(ALLOCATOR->stack + ndx, &NULL_POINTER, ndx);
            ALLOCATOR->count--;
        }
        ++ndx;
    }
}
void __alloc_terminate()
{
    if (ALLOCATOR == NULL)
    {
        return;
    }
    if (ALLOCATOR->stack != NULL)
    {
        __alloc_clear();
        free(ALLOCATOR->stack);
        ALLOCATOR->stack = NULL;
    }
    free(ALLOCATOR);
    ALLOCATOR = NULL;
}
void *__alloc_allocate(size_t size)
{
    void *ptr = malloc(size);

    if (ptr != NULL)
    {
        __stack_add_ptr(ptr);
    }

    return ptr;
}
bool __alloc_deallocate(void *ptr)
{
    return __stack_rem_ptr(ptr);
}
mem_ptr __alloc_get_pointer(uintptr_t addr)
{
    int ndx;
    return __stack_get_pointer(addr, &ndx);
}

const struct Open_Allocator Allocator = {
    .init = &__alloc_init,
    .count = &__alloc_count,
    .capacity = &__alloc_cap,
    .clear = &__alloc_clear,
    .terminate = &__alloc_terminate,
    .alloc = &__alloc_allocate,
    .dealloc = &__alloc_deallocate,
    .pointer = &__alloc_get_pointer};