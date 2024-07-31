#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/reftable.h"

static reftbl *table = NULL;

#define INITIAL_CAPACITY 16 // must not be zero

void rt_create(void);
rt_entry *create_ref(const char *type_name, void *t)
{
    rt_entry *entry = malloc(sizeof(rt_entry));
    entry->hash = 1010101;
    entry->type = String.alloc(type_name);
    entry->value = t;

    return entry;
}

reftbl *rt(void)
{
    if (table == NULL)
    {
        rt_create();
    }

    return table;
}

void rt_create(void)
{
    // Allocate space for ref table struct.
    table = malloc(sizeof(reftbl));
    if (table == NULL)
    {
        return;
    }
    table->length = 0;
    table->capacity = INITIAL_CAPACITY;
    table->ref = &create_ref;

    // Allocate (zero'd) space for entry buckets.
    table->entries = calloc(table->capacity, sizeof(rt_entry));
    if (table->entries == NULL)
    {
        free(table); // error, free table before we return!
        return;
    }
}

static bool tbl_is()
{
    return table != NULL;
}
static size_t tbl_length()
{
    return tbl_is() ? table->length : -1;
}

const struct RefTable_T RefTable = {
    .length = &tbl_length,
    .is = &tbl_is};