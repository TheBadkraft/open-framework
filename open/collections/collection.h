#ifndef _COLLECTION_H
#define _COLLECTION_H

#include <stdint.h>

#include "../core/core.h"

static const size_t DEFAULT_ARRAY_SIZE = 64;

// using array here because this can be accessed with the index operator `[x]`
typedef handle *array;

//  we always want to keep an empty element at the end
static const handle EMPTY_ELEMENT = 0;

typedef bool (*comparator)(object, object);

struct set_collection
{
    array bucket;
    size_t capacity;
    handle end;
};
struct set_enumerator
{
    struct set_collection *list;
    handle *element;
    object current;
};
struct coll_iterator
{
    struct set_enumerator *enumer;
    comparator comparer;
};

//  basic element bucket
typedef struct set_collection *collection;
//  basic element enumerator
typedef struct set_enumerator *enumerator;
//  element comparer enumeration
typedef struct set_iterator *iterator;

extern const struct ICollection
{
    collection (*new)(int cap);
    void (*dispose)(collection);
    size_t (*count)(collection);
    bool (*add)(collection, object);
    enumerator (*get_enumerator)(collection);
} Collection;
extern const struct IEnumerator
{
    bool (*next)(enumerator);
    void (*reset)(enumerator);
    void (*dispose)(enumerator);
} Enumerator;
#endif //  _COLLECTION_H