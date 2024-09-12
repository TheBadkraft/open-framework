#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define DEBUG 1
#endif

#include "../open/test.h"
#include "alloc_test.h"

#include "../open/core.h"
#include "../open/allocator.h"

typedef struct test1
{
    int id;
    string name;
} *target1;

typedef struct test2
{
    enum
    {
        ONE = 1,
        TWO,
        THREE
    } status;
    string name;
} *target2;

//  ALLOCATOR: test case prototypes
void allocate_item();
void get_mem_pointer();
void deallocate_item();
void multiple_allocs();

void __output_allocator_info();
void __output_pointer_info(mem_block);

int main(int argc, string *argv)
{
    BEGIN_SET(Allocatgor.API, true)
    {
        TEST(allocate_item);
        TEST(get_mem_pointer);
        TEST(deallocate_item);
        TEST(multiple_allocs);

        Allocator.terminate();
    }
    END_SET(Allocator.API);

    TEST_STATS();
}

void __output_allocator_info()
{
    writeln("============================================");
    writeln("Allocator:");
    writefln("  count:    %ld", Allocator.count());
    writefln("  capacity: %ld", Allocator.capacity());
    _output_alloc_stats();
    writeln("============================================");
}
void __output_pointer_info(mem_block memBlock)
{
    printf("MemPtr: ");
    if (memBlock != NULL)
    {
        puts("");
        writefln("  addr:   %ld", (uintptr_t)memBlock);
        writefln("  uptr:   %ld", memBlock->uptr);
        writefln("  size:   %ld", sizeof((void *)(memBlock->uptr)));
    }
    else
    {
        writeln("  NULL");
    }
}

//		================================ TEST CASES ================================
void allocate_item()
{
    writeln("Allocate mem for item");

    target2 t2 = Allocator.alloc(sizeof(struct test2), UNINITIALIZED);
    assert(t2 != NULL);
    assert(Allocator.count() == 1);

    __output_allocator_info();
    __output_pointer_info(_alloc_get_pointer((uintptr_t)t2));

    Allocator.flush();
}
void get_mem_pointer()
{
    writeln("Get memory pointer for item");

    target2 t2 = Allocator.alloc(sizeof(struct test2), INITIALIZED);
    t2->status = ONE;
    t2->name = "Henry";

    uintptr_t uptr = (uintptr_t)t2;
    mem_block memPtr = _alloc_get_pointer(uptr);
    assert(memPtr != NULL);

    target2 tx = (target2)memPtr->uptr;
    assert(t2->status == tx->status);
    assert(strcmp(t2->name, tx->name) == 0);

    __output_allocator_info();

    Allocator.flush();
}
void deallocate_item()
{
    writeln("Deallocate mem for item");

    target2 t2 = Allocator.alloc(sizeof(struct test2), INITIALIZED);

    __output_allocator_info();
    __output_pointer_info(_alloc_get_pointer((uintptr_t)t2));
    assert(Allocator.dealloc(t2));
    __output_allocator_info();
    __output_pointer_info(_alloc_get_pointer((uintptr_t)t2));

    Allocator.flush();
}
void multiple_allocs()
{
    writeln("Allocate multiple to verify expected behavior");

    int expId = 55;

    target2 t2A = Allocator.alloc(sizeof(struct test2), INITIALIZED);
    target1 t1A = Allocator.alloc(sizeof(struct test1), INITIALIZED);
    target2 t2B = Allocator.alloc(sizeof(struct test2), UNINITIALIZED);
    __output_allocator_info();

    t1A->id = expId;
    t1A->name = "James";

    uintptr_t uptr = (uintptr_t)t1A;
    __output_allocator_info();
    __output_pointer_info(_alloc_get_pointer(uptr));

    target1 actT1 = (target1)(_alloc_get_pointer(uptr)->uptr);
    assert(actT1 != NULL);
    assert(actT1->id == t1A->id);
    assert(strcmp(actT1->name, t1A->name) == 0);

    uptr = (uintptr_t)t2B;
    __output_pointer_info(_alloc_get_pointer(uptr));
    Allocator.dealloc(t2B);
    assert(_alloc_get_pointer(uptr) == NULL);
    writefln("Successfully Deallocated [%ld]!!!", uptr);
    __output_pointer_info(_alloc_get_pointer(uptr));

    __output_allocator_info();
    Allocator.flush();
}