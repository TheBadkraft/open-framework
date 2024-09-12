#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "open/allocator.h"

#if DEBUG
#include "test/alloc_test.h"
#endif

//  for some reason, placing this inside the DEFINE preproc isn't working for testing
static int ALLOC_COUNT = 0;
static int DEALLOC_COUNT = 0;

int counters[2];

void _alloc_incr_alloc_count()
{
    ALLOC_COUNT = ++counters[0];
}
void _alloc_incr_dealloc_count()
{
    DEALLOC_COUNT = ++counters[1];
}
void _flush_counters()
{
    counters[0] = 0;
    counters[1] = 0;
}

/*
    The goal is to eventually have the allocator so that it is not required for external code to initialize.
*/
static const uintptr_t NULL_ADDR_PTR = 0;
static const size_t DEFAULT_STACK_SIZE = 64;
static struct Mem_Page MEMPAGE = {
    .is_initialized = false,
    .capacity = 0,
    .count = 0,
    .page = NULL};
static struct Mem_Block EMPTY_BLOCK = {
    .uptr = NULL_ADDR_PTR};

//  PROTOTYPES
void page_fill();
void page_add_ptr(void *);
bool page_rem_ptr(void *);
bool page_rem_at(int);
uintptr_t page_repl_at(uintptr_t, int);
mem_block page_get_block(uintptr_t);

bool block_equ(mem_block, mem_block);
int block_cmp(mem_block, mem_block);

size_t alloc_count();
size_t alloc_capacity();
void alloc_flush();
void alloc_terminate();
void *alloc_allocate(size_t, allocMode);
bool alloc_deallocate(void *);

// #if DEBUG
mem_block _alloc_get_pointer(uintptr_t uptr)
{
    return page_get_block(uptr);
}
void _output_alloc_stats()
{
    // printf("total allocations:    %d\n", ALLOC_COUNT);
    // printf("total deallocations:  %d\n", DEALLOC_COUNT);
}
// #endif

bool has_mem_page(void)
{
    if (!MEMPAGE.is_initialized)
    {
        MEMPAGE.count = 0;
        MEMPAGE.capacity = DEFAULT_STACK_SIZE;
        MEMPAGE.page = malloc(sizeof(struct Mem_Block) * MEMPAGE.capacity);
        page_fill();

        MEMPAGE.is_initialized = true;

        // #if DEBUG
        _flush_counters();
        // #endif
    }

    return MEMPAGE.is_initialized;
}

//  block, mem_ptr functions are never called directly from the API so they will
//  assume memBlock is valid
void page_fill()
{
    //  this is naive in that it will not be reliable after (when) the memBlock is resized
    //  TODO: improve function when memBlock is resized
    mem_block ndx = MEMPAGE.page;
    size_t pos = ndx - MEMPAGE.page;

    // #if DEBUG
    // printf("Filling Page      %ld\n", EMPTY_BLOCK.uptr);
    // #endif

    while (pos < MEMPAGE.capacity)
    {
        uintptr_t uptr_cpy = (uintptr_t)memcpy(ndx, &EMPTY_BLOCK, sizeof(struct Mem_Block));
        // #if DEBUG
        // printf("[%2ld]    %ld\n", pos, uptr_cpy);
        // #endif

        pos = ++ndx - MEMPAGE.page;
    }

    // #if DEBUG
    // printf("---------------------------------------------------\n");
    // #endif
}
void page_add_ptr(void *ptr)
{
    size_t pos = -1;
    //  find the first empty block
    mem_block memBlk = page_get_block(NULL_ADDR_PTR);
    if (memBlk != NULL)
    {
        pos = memBlk - MEMPAGE.page;
        // memBlk->uptr = (uintptr_t)ptr;
        uintptr_t uaddr = (uintptr_t)(MEMPAGE.page + pos);
        // #if DEBUG
        // printf("Allocating        %ld\n", (uintptr_t)ptr);
        // #endif

        uaddr = page_repl_at((uintptr_t)ptr, pos);
        MEMPAGE.count++;

        _alloc_incr_alloc_count();
        // #if DEBUG
        // printf("Allocated         %ld\n", memBlk->uptr);
        // #endif
    }
    else
    {
        perror("Need to check allocation pool. It appears to be full.");
    }
}
bool page_rem_ptr(void *ptr)
{
    mem_block memBlk = page_get_block((uintptr_t)ptr);
    int pos = memBlk - MEMPAGE.page;
    bool retOk = pos >= 0 && pos < MEMPAGE.capacity;

    if (retOk)
    {
        uintptr_t uaddr = (uintptr_t)(MEMPAGE.page + pos);
        // #if DEBUG
        // printf("Deallocating     [%ld]    %ld\n", uaddr, memBlk->uptr);
        // #endif

        retOk = page_rem_at(pos);
        // #if DEBUG
        // printf("Deallocated      [%ld]    %ld\n", uaddr, memBlk->uptr);
        // #endif
    }
    if (retOk)
    {
        free(ptr);
        _alloc_incr_dealloc_count();
    }

    return retOk;
}
bool page_rem_at(int pos)
{
    page_repl_at(NULL_ADDR_PTR, pos);
    MEMPAGE.count--;

    //  TODO: determine how we can validate the function
    return true;
}
uintptr_t page_repl_at(uintptr_t memPtr, int pos)
{
    uintptr_t uaddr = (uintptr_t)(MEMPAGE.page + pos);
    // #if DEBUG
    // printf("Replacing at %2d: [%ld] -> %ld\n", pos, uaddr, (MEMPAGE.page + pos)->uptr);
    // #endif

    (MEMPAGE.page + pos)->uptr = memPtr;

    // #if DEBUG
    // printf("Replaced  at %2d: [%ld] <- %ld\n", pos, uaddr, (MEMPAGE.page + pos)->uptr);
    // #endif

    return uaddr;
}
mem_block page_get_block(uintptr_t uptr)
{
    mem_block ndx = MEMPAGE.page;
    size_t pos = ndx - MEMPAGE.page;
    mem_block memPtr = NULL;

    while (pos < MEMPAGE.capacity)
    {
        if (ndx->uptr == uptr)
        {
            memPtr = ndx;

            break;
        }
        ++ndx;
        pos = ndx - MEMPAGE.page;
    }

    return memPtr;
}

int block_cmp(mem_block mp1, mem_block mp2)
{
    int ret = mp1->uptr == mp2->uptr  ? 0
              : mp1->uptr < mp2->uptr ? -1
                                      : 1;

    // if (mp1->addr < mp2->addr)
    // {
    //     ret = -1;
    // }
    // else if (mp1->addr > mp2->addr)
    // {
    //     ret = 1;
    // }

    return ret;
}
bool block_equ(mem_block mp1, mem_block mp2)
{
    int cmpRet = block_cmp(mp1, mp2);
    return cmpRet == 0;
}

//  user-facing API: functions need ensure MEMPAGE is initialized
size_t alloc_count()
{
    if (!has_mem_page())
    {
        perror("Allocator MemPage Initialization failure");
    }

    return MEMPAGE.count;
}
size_t alloc_capacity()
{
    if (!has_mem_page())
    {
        perror("Allocator MemPage Initialization failure");
    }

    return MEMPAGE.capacity;
}
void alloc_flush()
{
    if (!has_mem_page())
    {
        perror("Allocator MemPage Initialization failure");
    }

    // #if DEBUG
    // printf("Flushing Allocator MemPage\n");
    // #endif

    //  for each memPtr: remove & free
    mem_block ndx = MEMPAGE.page;
    size_t pos = ndx - MEMPAGE.page;

    while (pos < MEMPAGE.capacity)
    {
        if (!block_equ(&EMPTY_BLOCK, ndx))
        {
            page_rem_at(pos);
            void *ptr = (void *)(ndx->uptr);
            free(ptr);
            _alloc_incr_dealloc_count();
        }
        ++ndx;
        pos = ndx - MEMPAGE.page;
    }

    // #if DEBUG
    _output_alloc_stats();
    // #endif

    //  may not necessarily want to automatically flush counters; at this point, if there is
    //  an imbalance of allocations/deallocations we may want to throw or ... ???
    _flush_counters();
}
void alloc_terminate()
{
    if (!has_mem_page())
    {
        perror("Allocator MemPage Initialization failure");
    }

    if (MEMPAGE.page != NULL)
    {
        alloc_flush();
        free(MEMPAGE.page);
        MEMPAGE.page = NULL;
        MEMPAGE.is_initialized = false;
    }
}
void *alloc_allocate(size_t size, allocMode mode)
{
    if (!has_mem_page())
    {
        perror("Allocator MemPage Initialization failure");
    }

    void *ptr;
    switch (mode)
    {
    case UNINITIALIZED:
        ptr = malloc(size);

        break;
    case INITIALIZED:
        ptr = calloc(1, size);

        break;
    }

    if (ptr != NULL)
    {
        page_add_ptr(ptr);
    }

    return ptr;
}
bool alloc_deallocate(void *ptr)
{
    if (!has_mem_page())
    {
        perror("Allocator MemPage Initialization failure");
    }

    return page_rem_ptr(ptr);
}

const struct Open_Allocator Allocator = {
    .count = &alloc_count,
    .capacity = &alloc_capacity,
    .flush = &alloc_flush,
    .terminate = &alloc_terminate,
    .alloc = &alloc_allocate,
    .dealloc = &alloc_deallocate};