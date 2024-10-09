#ifndef _COLLECTION_H
#define _COLLECTION_H

#include <stdint.h>

#include "../core/core.h"

static const size_t DEFAULT_ARRAY_SIZE = 64;

// using array here because this can be accessed with the index operator `[x]`
typedef handle *array;

//  we always want to keep an empty element at the end
static const handle EMPTY_ELEMENT = 0;

extern const struct set_cap
{
    handle cap;
} CAP;

typedef bool (*comparator)(object, object);

struct set_collection
{
    array bucket;
    handle end;
    size_t capacity;
};
struct set_enumerator
{
    struct set_collection *list;
    handle *element;
    object current;
};
struct query_iterator
{
    struct set_enumerator *enumer;
    bool (*compare)(object, object);
};

//  basic element bucket
typedef struct set_collection *collection;
//  basic element enumerator
typedef struct set_enumerator *enumerator;
//  basic query iterator
typedef struct query_iterator *iterator;

extern const struct ICollection
{
    collection (*new)(void);
    void (*dispose)(collection);
    size_t (*count)(collection);
    size_t (*capacity)(collection);
    object (*add)(collection, object);
    object (*remove)(iterator, object);
    enumerator (*get_enumerator)(collection);
    bool (*get_queryable)(collection, iterator *);
} Collection;
extern const struct IEnumerator
{
    bool (*next)(enumerator);
    void (*reset)(enumerator);
    void (*dispose)(enumerator);
} Enumerator;
extern const struct IIterator
{
    void (*dispose)(iterator);
} Iterator;

#endif //  _COLLECTION_H