#include <stdio.h>
#include <stdlib.h>

#include "../open/test.h"

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

int main(int argc, string *argv)
{
    BEGIN_SET(Allocatgor.API, true)
    {
        Allocator.init();
        TEST(allocate_item);
        Allocator.terminate();
        Allocator.init();
        TEST(get_mem_pointer);
        TEST(deallocate_item);

        Allocator.terminate();
    }
    END_SET(Allocator.API);

    TEST_STATS();
}

void __output_allocator_info(mem_ptr ptr)
{
    writeln("Allocator:");
    writefln("  count:    %ld", Allocator.count());
    writefln("  capacity: %ld", Allocator.capacity());
    if (ptr != NULL)
    {
        writeln("    MemPtr: ");
        writefln("      addr:   %ld", ptr->addr);
        writefln("      size:   %ld", sizeof(ptr));
    }
}

//		================================ TEST CASES ================================
void allocate_item()
{
    writeln("Allocate mem for item");

    __output_allocator_info(NULL);
    target2 t2 = Allocator.alloc(sizeof(struct test2));
    assert(t2 != NULL);
    assert(Allocator.count() == 1);
    mem_ptr ptr = malloc(sizeof(struct Mem_Pointer));
    ptr->item = t2;
    ptr->addr = (uintptr_t)t2;

    __output_allocator_info(ptr);

    Allocator.clear();
}
void get_mem_pointer()
{
    writeln("Get memory pointer for item");

    target2 t2 = Allocator.alloc(sizeof(struct test2));
    t2->status = ONE;
    t2->name = "Henry";
    uintptr_t addr = (uintptr_t)t2;

    mem_ptr memPtr = Allocator.pointer(addr);
    assert(memPtr != NULL);

    target2 tx = (target2)memPtr->item;
    assert(t2->status == tx->status);
    assert(strcmp(t2->name, tx->name) == 0);

    __output_allocator_info(memPtr);

    Allocator.clear();
}
void deallocate_item()
{
    writeln("Deallocate mem for item");

    target2 t2 = Allocator.alloc(sizeof(struct test2));

    mem_ptr memPtr = malloc(sizeof(struct Mem_Pointer));
    memPtr->item = t2;
    memPtr->addr = (uintptr_t)t2;

    __output_allocator_info(memPtr);
    assert(Allocator.dealloc(t2));
    __output_allocator_info(NULL);

    free(memPtr);
    Allocator.clear();
}