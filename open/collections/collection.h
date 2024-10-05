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
typedef object (*enumerator)(void);

struct coll
{
    array list;
    size_t capacity;
    handle last;
    const enumerator get_enumerator;
};
struct iter
{
    handle current;
    comparator compare;
};

typedef struct coll *collection;
typedef struct iter *iterator;

extern const struct ICollection
{
    collection (*new)(int cap);
    void (*dispose)(collection);
    size_t (*count)(collection);
    bool (*add)(collection, object);
    iterator (*get_iterator)(collection, comparator);
} Collection;
#endif //  _COLLECTION_H