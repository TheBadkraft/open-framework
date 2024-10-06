#include <stdio.h>

#include "open/collections/collection.h"
#include "open/core/allocator.h"

const float RESIZE_THRESHOLD = .75F;

/*
    NEXT:   short-term features
        - remove
        - arrange
        - resize
        - clear
        - iterator
        - copy-to
        - copy-from
        - add-range
        - insert-at
        - remove-at
        - get-at
        - alloc


    TODO:   long-term enhancements
        - ~~replace collection->capacity with `cap` pointer~~
        - replace enumerator->list with ref id
        - internally manage enumerator ref properties
        - interrupt enumerators/iterators if collection modified
        - implement a yield pattern
*/

//  === internal prototypes

void _array_allocate(array *, size_t *, size_t);
void _array_resize(array *, size_t *);

bool _coll_at_threshold(collection);

//  === interface prototypes

/// @brief create a new collection
/// @param cap      initial collection capacity; if lower than default, will resolve to initial default
/// @return TRUE if object was added; otherwise FALSE
collection coll_new(int);
/// @brief dispose of the current collection
/// @param list     the object to dispose (free)
void coll_dispose(collection);
size_t coll_count(collection);
bool coll_add_item(collection, object);
enumerator coll_get_enumerator(collection);
bool enumer_move_next(enumerator);
void enumer_reset(enumerator);

iterator coll_get_iterator(collection, comparator);

//  === internal functions

/// @brief allocate memory for the current array list
/// @param list     the target list
/// @param current  the current list capacity; both in and out
/// @param count    the list count
void _array_allocate(array *list, size_t *current, size_t count)
{
    /*
        Want to put a little smarts in the allocation of memory and resizing capacity.
        We will pass the list, current capacity, and count ... I don't see the
        point in blindly doubling the current capacity. The larger the capacity, the more will
        be increased by some factor. We will always increase capacity by +1 to accomodate the
        empty element. Neither count nor capacity will (should) never include the last (end)
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
/// @brief resize memory allocation for the current array list
/// @param list     the target list
/// @param current  current list capacity; both in and out
void _array_resize(array *list, size_t *current)
{
    //  TODO ...
}
/// @brief determines if the collection is at threshold for resizing
/// @param list the collection
/// @return TRUE if resize; otherwise FALSE
bool _coll_at_threshold(collection list)
{
    /*
        This is not necessarily a binary option. We could determine that we want the collection
        capacity to shrink. And then ... by how much???
    */
    size_t ratio = coll_count(list) / list->capacity;
    return ratio > RESIZE_THRESHOLD;
}

//  === interface functions

/// @brief
/// @param cap
/// @return
collection coll_new(int cap)
{
    cap = cap < DEFAULT_ARRAY_SIZE ? DEFAULT_ARRAY_SIZE : cap;

    collection list = Allocator.alloc(sizeof(struct set_collection), UNINITIALIZED);
    list->capacity = 0;
    list->bucket = NULL;

    _array_allocate(&(list->bucket), &(list->capacity), coll_count(list));
    //  hold reference to the last (end) item so we can calculate count
    list->end = (handle)(list->bucket);

    // printf("(%p ... %p ... %p) <- %p\n", list->list, (list->list + count), (object)(list->end), item);

    return list;
}
void coll_dispose(collection list)
{
    Allocator.dealloc(list->bucket);
    list->capacity = 0;
    list->end = (handle)list->bucket;

    // printf("(%p ... %p ... %p) <- %p\n", list->list, (list->list + count), (object)(list->end), item);

    Allocator.dealloc(list);

    list = NULL;
}
size_t coll_count(collection list)
{
    handle memDiff = list->end - (handle)list->bucket;

    return memDiff / sizeof(handle);
}
bool coll_add_item(collection list, object item)
{
    bool retAdd = false;
    size_t count = coll_count(list);

    if (_coll_at_threshold(list))
    {
        _array_resize(&(list->bucket), &(list->capacity));
    }
    retAdd = (list->end + sizeof(handle)) < (handle)(list->bucket + list->capacity);

    if (retAdd)
    {
        (*(list->bucket + count)) = (handle)item;
        list->end += sizeof(handle);

        // printf("(%p ... %p ... %p) <- %p\n", list->list, (list->list + count), (object)(list->end), item);

        retAdd = coll_count(list) - count == 1;
    }

    return retAdd;
}
enumerator coll_get_enumerator(collection list)
{
    enumerator pEnumerator = Allocator.alloc(sizeof(struct set_enumerator), UNINITIALIZED);
    pEnumerator->list = list;
    pEnumerator->element = NULL;
    pEnumerator->current = NULL;
}

bool enumer_move_next(enumerator pEnumerator)
{
    handle *end = (handle *)pEnumerator->list->end;
    handle *current = pEnumerator->element;

    if (current == NULL)
    {
        //  ASSUMPTION: we're at the beginning of the enumeration
        current = pEnumerator->list->bucket;
    }
    else if (current != end)
    {
        ++current;
    }
    else
    {
        //  we're not sure what went wrong to end up here but stop the presses!!!
        return false;
    }
    // printf("move next --> %p\n", current);

    pEnumerator->element = current;
    pEnumerator->current = (object)*current;

    return current != end;
}
void enumer_reset(enumerator pEnumerator)
{
    pEnumerator->element = NULL;
    pEnumerator->current = NULL;
}
void enumer_dispose(enumerator pEnumerator)
{
    Allocator.dealloc(pEnumerator);
}

const struct ICollection Collection = {
    .new = &coll_new,
    .dispose = &coll_dispose,
    .count = &coll_count,
    .add = &coll_add_item,
    .get_enumerator = &coll_get_enumerator,
};
const struct IEnumerator Enumerator = {
    .next = &enumer_move_next,
    .reset = &enumer_reset,
    .dispose = &enumer_dispose,
};
