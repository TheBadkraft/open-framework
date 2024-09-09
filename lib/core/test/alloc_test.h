#ifndef _ALLOC_TEST_H
#define _ALLOC_TEST_H

#include <stdio.h>

#include "../open/allocator.h"

mem_block _alloc_get_pointer(uintptr_t);
void _output_alloc_stats();

#endif //  _ALLOC_TEST_H