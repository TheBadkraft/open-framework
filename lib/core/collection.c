#include <stdio.h>

#include "open/collections/collection.h"
#include "open/core/allocator.h"

const float RESIZE_THRESHOLD = .75F;

//  internal prototypes

/// @brief allocate memory for the current array list
/// @param list     the target list
/// @param current  the current list capacity; both in and out
/// @param count    the list count
void _array_allocate(array *, size_t *, size_t);
/// @brief resize memory allocation for the current array list
/// @param list     the target list
/// @param current  current list capacity; both in and out
void _array_resize(array *, size_t *);

bool _coll_at_threshold(collection);

//  interface prototypes

/// @brief create a new collection
/// @param cap      initial collection capacity; if lower than default, will resolve to initial default
/// @return TRUE if object was added; otherwise FALSE
collection coll_new(int);
/// @brief dispose of the current collection
/// @param coll     the object to dispose (free)
void coll_dispose(collection);
size_t coll_count(collection);
bool coll_add_item(collection, object);

iterator coll_get_iterator(collection, comparator);

//  internal functions
void _array_allocate(array *list, size_t *current, size_t count)
{
    /*
        Want to put a little smarts in the allocation of memory and resizing capacity.
        We will pass the list, current capacity, and count ... I don't see the
        point in blindly doubling the current capacity. The larger the capacity, the more will
        be increased by some factor. We will always increase capacity by +1 to accomodate the
        empty element. Neither count nor capacity will (should) never include the last empty
        element.

        current   factor
        (64  * 1.5) + 1: 97
        (97  * 1.5) + 1: 147
        (147 * 1.5) + 1: 222
    */
    if ((*current) == 0)
    {
        //  new list
        (*list) = Allocator.alloc(((*current) = DEFAULT_ARRAY_SIZE) + 1, UNINITIALIZED);
    }
    else if (count / (*current) > RESIZE_THRESHOLD)
    {
        _array_resize(list, current);
    }
}
void _array_resize(array *list, size_t *current)
{
    //  TODO ...
}
bool _coll_at_threshold(collection coll)
{
    size_t ratio = coll_count(coll) / coll->capacity;
    return ratio > RESIZE_THRESHOLD;
}

//  interface functions
collection coll_new(int cap)
{
    cap = cap < DEFAULT_ARRAY_SIZE ? DEFAULT_ARRAY_SIZE : cap;

    collection coll = Allocator.alloc(sizeof(struct coll), UNINITIALIZED);
    coll->capacity = 0;
    coll->list = NULL;

    _array_allocate(&(coll->list), &(coll->capacity), coll_count(coll));
    //  hold reference to the last item so we can calculate count
    coll->last = (handle)(coll->list);

    // printf("list -> %p\n", coll->list);
    // printf("last -> %p\n", (object)(coll->last));

    return coll;
}
void coll_dispose(collection coll)
{
    Allocator.dealloc(coll->list);
    coll->capacity = 0;
    coll->last = (handle)coll->list;

    // printf("list -> %p\n", coll->list);
    // printf("last -> %p\n", (object)(coll->last));

    Allocator.dealloc(coll);

    coll = NULL;
}
size_t coll_count(collection coll)
{
    handle memDiff = coll->last - (handle)coll->list;

    return memDiff / sizeof(handle);
}
bool coll_add_item(collection coll, object item)
{
    bool retAdd = false;
    size_t count = coll_count(coll);

    if (_coll_at_threshold(coll))
    {
        _array_resize(&(coll->list), &(coll->capacity));
    }
    retAdd = (coll->last + sizeof(handle)) < (handle)(coll->list + coll->capacity);
    // handle *pElement = coll->list;
    // if (count == 0)
    // {
    //     // coll->list[0] = (handle)item;
    //     // coll->last = (handle)(&(coll->list[1]));
    //     (*pElement) = (handle)item;
    //     coll->last = (handle)++pElement;

    //     printf("(%p ... %p)\n", coll->list, (object)(coll->last));

    //     // could be more robust but this works for now
    //     retAdd = coll_count(coll) - count == 1;
    // }

    if (retAdd)
    {
        (*(coll->list + count)) = (handle)item;
        coll->last += sizeof(handle);

        printf("(%p ... %p ... %p) <- %p\n", coll->list, (coll->list + count), (object)(coll->last), item);

        retAdd = coll_count(coll) - count == 1;
    }

    return retAdd;
}
iterator coll_get_iterator(collection coll, comparator comparer)
{
}

const struct ICollection Collection = {
    .new = &coll_new,
    .dispose = &coll_dispose,
    .count = &coll_count,
    .add = &coll_add_item,
    .get_iterator = &coll_get_iterator,
};
