// Simple hash table implemented in C.

#ifndef _REFTABLE_H
#define _REFTABLE_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdbool.h>

#include "cstring.h"

#define writeln(statement) ({ printf("%s\n", statement); strlen(statement); })

// ref table entry (slot may be filled or empty).
typedef struct
{
    signed long hash; //    hash is 0 if this slot is empty
    string *type;
    void *value;
} rt_entry;

// ref obj table structure: create with rt_create, free with rt_destroy.
typedef struct
{
    rt_entry *entries; // ref obj slots
    size_t capacity;   // size of _entries array
    size_t length;     // number of items in hash table
    rt_entry *(*ref)(const char *, void *);
} reftbl;

reftbl *rt(void);
void rt_destroy(reftbl *table);

// void *rt_get(reftbl *table, const char *key);

// const char *rt_set(reftbl *table, const char *key, void *value);

//  TODO ...
// Hash table iterator: create with ht_iterator, iterate with ht_next.
typedef struct
{
    const char *key; // current key
    void *value;     // current value

    // Don't use these fields directly.
    reftbl *_table; // reference to hash table being iterated
    size_t _index;  // current index into ht._entries
} rti;

// Return new hash table iterator (for use with ht_next).
rti rt_iterator(reftbl *table);

// Move iterator to next item in hash table, update iterator's key
// and value to current item, and return true. If there are no more
// items, return false. Don't call ht_set during iteration.
bool rt_next(rti *it);

extern const struct RefTable_T
{
    void (*clear)();
    void (*destroy)();
    size_t (*length)();
    bool (*is)();
    bool (*has_type)(const char *type_name);
    void (*add_type)(const char *type_name, const char *source);
} RefTable;

#endif // _REFTABLE_H