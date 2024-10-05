#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "open/core/allocator.h"

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
static const handle NULL_ADDR_PTR = 0;
static const size_t DEFAULT_PAGE_SIZE = 64;
static struct Mem_Page MEMPAGE = {
    .is_initialized = false,
    .capacity = 0,
    .count = 0,
    .page = NULL};
static struct Mem_Block EMPTY_BLOCK = {
    .obj_ptr = NULL_ADDR_PTR};

//  PROTOTYPES
void page_fill();
void page_add_ptr(object);
bool page_rem_ptr(object);
bool page_rem_at(int);
handle page_repl_at(handle, int);
mem_block page_get_block(handle);

bool block_equ(mem_block, mem_block);
int block_cmp(mem_block, mem_block);

size_t alloc_count();
size_t alloc_capacity();
void alloc_flush();
void alloc_terminate();
object alloc_allocate(size_t, allocMode);
bool alloc_deallocate(object);
bool object_is_alloc(object);

// #if DEBUG
mem_block _alloc_get_pointer(handle objPtr)
{
    return page_get_block(objPtr);
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
        MEMPAGE.capacity = DEFAULT_PAGE_SIZE;
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
    mem_block blockNdx = MEMPAGE.page;
    size_t pos = blockNdx - MEMPAGE.page;

    // #if DEBUG
    // printf("Filling Page      %ld\n", EMPTY_BLOCK.objPtr);
    // #endif

    while (pos < MEMPAGE.capacity)
    {
        handle objPtr_cpy = (handle)memcpy(blockNdx, &EMPTY_BLOCK, sizeof(struct Mem_Block));
        // #if DEBUG
        // printf("[%2ld]    %ld\n", pos, objPtr_cpy);
        // #endif

        pos = ++blockNdx - MEMPAGE.page;
    }

    // #if DEBUG
    // printf("---------------------------------------------------\n");
    // #endif
}
void page_add_ptr(object obj)
{
    size_t pos = -1;
    //  find the first empty block
    mem_block memBlock = page_get_block(NULL_ADDR_PTR);
    if (memBlock != NULL)
    {
        pos = memBlock - MEMPAGE.page;
        // memBlock->objPtr = (obj_ptr)ptr;
        /*  //  I don't recall if I did this for logging purposes but seems useless...
            handle objPtr = (handle)(MEMPAGE.page + pos);
        */
        // #if DEBUG
        // printf("Allocating        %ld\n", (obj_ptr)ptr);
        // #endif

        handle objPtr = page_repl_at((handle)obj, pos);
        MEMPAGE.count++;

        _alloc_incr_alloc_count();
        // #if DEBUG
        // printf("Allocated         %ld\n", memBlock->objPtr);
        // #endif
    }
    else
    {
        perror("Need to check allocation pool. It appears to be full.");
    }
}
bool page_rem_ptr(object obj)
{
    mem_block memBlock = page_get_block((handle)obj);
    int pos = memBlock - MEMPAGE.page;
    bool retOk = pos >= 0 && pos < MEMPAGE.capacity;

    if (retOk)
    {
        // #if DEBUG
        // handle objPtr = (handle)(MEMPAGE.page + pos);
        // printf("Deallocating     [%ld]    %ld\n", uaddr, memBlock->objPtr);
        // #endif

        retOk = page_rem_at(pos);
        // #if DEBUG
        // printf("Deallocated      [%ld]    %ld\n", uaddr, memBlock->objPtr);
        // #endif
    }
    if (retOk)
    {
        free(obj);
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
handle page_repl_at(handle objPtr, int pos)
{
    handle blockPtr = (handle)(MEMPAGE.page + pos);
    // #if DEBUG
    // printf("Replacing at %2d: [%ld] -> %ld\n", pos, uaddr, (MEMPAGE.page + pos)->objPtr);
    // #endif

    (MEMPAGE.page + pos)->obj_ptr = objPtr;

    // #if DEBUG
    // printf("Replaced  at %2d: [%ld] <- %ld\n", pos, uaddr, (MEMPAGE.page + pos)->objPtr);
    // #endif

    return blockPtr;
}
mem_block page_get_block(handle objPtr)
{
    mem_block blockNdx = MEMPAGE.page;
    size_t pos = blockNdx - MEMPAGE.page;
    mem_block memBlock = NULL;

    while (pos < MEMPAGE.capacity)
    {
        if (blockNdx->obj_ptr == objPtr)
        {
            memBlock = blockNdx;

            break;
        }
        ++blockNdx;
        pos = blockNdx - MEMPAGE.page;
    }

    return memBlock;
}

int block_cmp(mem_block mb1, mem_block mb2)
{
    int ret = mb1->obj_ptr == mb2->obj_ptr  ? 0
              : mb1->obj_ptr < mb2->obj_ptr ? -1
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
bool block_equ(mem_block mb1, mem_block mb2)
{
    int cmpRet = block_cmp(mb1, mb2);
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

    //  for each objPtr: remove & dispose
    mem_block blockNdx = MEMPAGE.page;
    size_t pos = blockNdx - MEMPAGE.page;

    while (pos < MEMPAGE.capacity)
    {
        if (!block_equ(&EMPTY_BLOCK, blockNdx))
        {
            page_rem_at(pos);
            object obj = (object)(blockNdx->obj_ptr);
            free(obj);

            _alloc_incr_dealloc_count();
        }
        ++blockNdx;
        pos = blockNdx - MEMPAGE.page;
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
object alloc_allocate(size_t size, allocMode mode)
{
    if (!has_mem_page())
    {
        perror("Allocator MemPage Initialization failure");
    }

    object obj;
    switch (mode)
    {
    case UNINITIALIZED:
        obj = malloc(size);

        break;
    case INITIALIZED:
        obj = calloc(1, size);

        break;
    }

    if (obj != NULL)
    {
        page_add_ptr(obj);
    }

    return obj;
}
bool alloc_deallocate(object obj)
{
    if (!has_mem_page())
    {
        perror("Allocator MemPage Initialization failure");
    }

    return page_rem_ptr(obj);
}
bool object_is_alloc(object obj)
{
    handle objPtr = (handle)obj;
    return page_get_block(objPtr)->obj_ptr != NULL_ADDR_PTR;
}

const struct IAllocator Allocator = {
    .count = &alloc_count,
    .capacity = &alloc_capacity,
    .flush = &alloc_flush,
    .terminate = &alloc_terminate,
    .alloc = &alloc_allocate,
    .dealloc = &alloc_deallocate};