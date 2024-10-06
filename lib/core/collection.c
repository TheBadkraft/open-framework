#include <stdio.h>

#include "open/collections/collection.h"
#include "open/core/allocator.h"

const float RESIZE_THRESHOLD = .75F;
const size_t HANDLE_SIZE = sizeof(handle);

const struct set_cap CAP = {
    .cap = (handle)NULL,
};

/*
    TODO:
      I. interface
        [ ] remove
        [x] capacity -- recalculates array capacity
        [ ] resize
        [ ] clear
        [ ] iterator
        [ ] sort
        [ ] find
        [ ] copy-to
        [ ] copy-from
        [ ] add-range
        [ ] insert-at
        [ ] remove-at
        [ ] get-at
        [ ] alloc
        [ ] add (change to return the object added instead of bool)
      II. enhancements
        [x] enhance collection->capacity
        [ ] replace enumerator->list with ref id
        [ ] internally manage enumerator ref properties
        [ ] interrupt enumerators/iterators if collection modified
        [ ] implement a yield pattern
*/

//  === internal prototypes

void _array_allocate(array *, size_t *, size_t);
size_t _array_resize(array *);
handle *_array_get_cap_loc(array, size_t);
bool _array_at_threshold(size_t, size_t);

//  === interface prototypes

collection coll_new(void);
void coll_dispose(collection);
size_t coll_count(collection);
size_t coll_capacity(collection);
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
    if ((*current) == 0)
    {
        //  new list
        (*current) = DEFAULT_ARRAY_SIZE;
        (*list) = Allocator.alloc((*current) + 1, UNINITIALIZED);
        (*list)[*current + 1] = (handle)(&CAP);
    }
    else if (count / (*current) > RESIZE_THRESHOLD)
    {
        (*current) = _array_resize(list);
    }
}
/// @brief resize memory allocation for the current array list
/// @param list     the target list
/// @return new array capacity
size_t _array_resize(array *list)
{
    //  TODO ...
    return -1;
}
/// @brief determines if the collection is at threshold for resizing
/// @param count collection's element count
/// @return TRUE if resize; otherwise FALSE
bool _array_at_threshold(size_t count, size_t capacity)
{
    /*
        This is not necessarily a binary option. We could determine that we want the collection
        capacity to shrink. And then ... by how much???
    */
    size_t ratio = count / capacity;
    return ratio > RESIZE_THRESHOLD;
}
/// @brief get the array's end cap element location
/// @param list the current array
/// @param count array element count
/// @return the cap object's element location
handle *_array_get_cap_loc(array list, size_t count)
{
#ifdef DEBUG
    printf("===== *** WARNING: calc array cap location *** =====\n\n");
#endif

    handle cap = (handle)(&CAP);
    handle *ptr = (list + count);

    bool retCont = true;

    while ((*ptr) != cap)
    {
        ++ptr;
    }

    return ptr;
}

//  === interface functions

/// @brief construct a new collection object
/// @return collection object; NULL if fail
collection coll_new(void)
{
    collection list = Allocator.alloc(sizeof(struct set_collection), INITIALIZED);
    if (list)
    {
        size_t capacity = 0;
        list->bucket = NULL;

        _array_allocate(&(list->bucket), &capacity, coll_count(list));
        //  hold reference to the last (end) item so we can calculate count
        list->end = (handle)(list->bucket);
        list->capacity = capacity;

        /*  -- debugging checks
            handle *cap = _array_get_cap_loc(list->bucket, 0);
            printf("%p ... %p ... %p (%ld)\n", list->bucket, (object)list->end, cap, coll_capacity(list));
         */
    }

    return list;
}
/// @brief dispose of the current collection
/// @param list     the object to dispose (free)
void coll_dispose(collection list)
{
    Allocator.dealloc(list->bucket);
    list->bucket = NULL;
    list->end = (handle)list->bucket;

    // printf("(%p ... %p ... %p) <- %p\n", list->list, (list->list + count), (object)(list->end), item);

    Allocator.dealloc(list);
    list = NULL;
}
/// @brief calculate element count
/// @param list current collectin
/// @return element count
size_t coll_count(collection list)
{
    handle memDiff = list->end - (handle)list->bucket;

    return memDiff / HANDLE_SIZE;
}
/// @brief re-calculates collection capacity
/// @param list current collection
/// @return collection capacity
size_t coll_capacity(collection list)
{
    handle *cap = _array_get_cap_loc(list->bucket, coll_count(list));
    list->capacity = (--cap) - list->bucket;

    return list->capacity;
}
/// @brief add element to collection
/// @param list current collection
/// @param item item to add
/// @return TRUE if added; otherwise FALSE
bool coll_add_item(collection list, object item)
{
    bool retAdd = false;
    size_t count = coll_count(list);

    if (!(retAdd = !_array_at_threshold(count, list->capacity)))
    {
        _array_resize(&(list->bucket));
        retAdd = true;
    }

    if (retAdd)
    {
        (*(list->bucket + count)) = (handle)item;
        list->end += HANDLE_SIZE;

        // printf("(%p ... %p ... %p) <- %p\n", list->list, (list->list + count), (object)(list->end), item);

        retAdd = coll_count(list) - count == 1;
    }

    return retAdd;
}
/// @brief create a collection enumerator
/// @param list current collection
/// @return enumerator
enumerator coll_get_enumerator(collection list)
{
    enumerator pEnumerator = Allocator.alloc(sizeof(struct set_enumerator), UNINITIALIZED);
    pEnumerator->list = list;
    pEnumerator->element = NULL;
    pEnumerator->current = NULL;
}

/// @brief advance enumerator one element
/// @param pEnumerator current enumerator
/// @return TRUE if enumerator can advance; otherwise FALSE
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
/// @brief reset the enumerator
/// @param pEnumerator current enumerator
void enumer_reset(enumerator pEnumerator)
{
    pEnumerator->element = NULL;
    pEnumerator->current = NULL;
}
/// @brief dispose of the enumerator
/// @param pEnumerator current enumerator
void enumer_dispose(enumerator pEnumerator)
{
    Allocator.dealloc(pEnumerator);
}

const struct ICollection Collection = {
    .new = &coll_new,
    .dispose = &coll_dispose,
    .count = &coll_count,
    .capacity = &coll_capacity,
    .add = &coll_add_item,
    .get_enumerator = &coll_get_enumerator,
};
const struct IEnumerator Enumerator = {
    .next = &enumer_move_next,
    .reset = &enumer_reset,
    .dispose = &enumer_dispose,
};
