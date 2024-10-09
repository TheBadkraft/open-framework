#include <stdio.h>
#include <string.h>

#include "open/collections/collection.h"
#include "open/core/allocator.h"
#include "open/core/diagnostics.h"

const float RESIZE_THRESHOLD = .75F;
const size_t HANDLE_SIZE = sizeof(handle);

const struct set_cap CAP = {
    .cap = (handle)NULL,
};

/*
    TODO:
      I. interface
        [x] remove
        [ ] delete
        [x] capacity -- recalculates array capacity
        [ ] resize
        [!] clear
        [x] iterator -- query iterator (queryable)
        [ ] sort
        [ ] find
        [ ] copy-to
        [ ] copy-from
        [ ] add-range
        [ ] insert-at
        [ ] remove-at
        [ ] get-at
        [ ] alloc
        [x] add (change to return the object added instead of bool)
      II. enhancements
        [x] enhance collection->capacity
        [ ] implement a yield pattern
        [ ] replace enumerator->list with ref id
        [ ] internally manage enumerator ref properties
        [ ] interrupt enumerators/iterators if collection modified
*/

void _array_allocate(array *, size_t *, size_t);
size_t _array_resize(array *);
handle *_array_get_cap_loc(array, size_t);
bool _array_at_threshold(size_t, size_t);
handle _array_remove_at(array, size_t);

collection list_new(void);
void list_dispose(collection);
size_t list_count(collection);
size_t list_capacity(collection);
object list_add_item(collection, object);
object list_remove_item(iterator, object);
enumerator list_get_enumerator(collection);
bool list_get_queryable(collection, iterator *);
bool enumer_move_next(enumerator);
void enumer_reset(enumerator);

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
        //  memset(mem, 0x12, 1024)
        memset((*list), EMPTY_ELEMENT, (*current));
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
/// @brief removes item from array using handle
/// @param list current array
/// @param index element location to be removed
/// @return handle of the end element
handle _array_remove_at(array list, size_t index)
{
    handle *current = (list + index);
    printf("removing at [%ld]: [%p (%ld)]\n", index, (list + index), (handle) * (list + index));

    while (*(++current) != EMPTY_ELEMENT)
    {
        printf("moving: [%p (%ld)] -> [%ld: %p]\n", current, (handle)*current, index, (list + index));

        *(list + index) = *current;

        printf("replaced: [%ld] <- [%p (%ld)]\n", index, (list + index), (handle) * (list + index));

        ++index;
    }

    *(list + index) = EMPTY_ELEMENT;

    printf("replaced: [%ld] <- [%p (%ld)]\n", index, (list + index), (handle) * (list + index));

    handle end = (handle)(list + index);
    return end;
}

/// @brief construct a new collection object
/// @return collection object; NULL if fail
collection list_new(void)
{
    collection list = Allocator.alloc(sizeof(struct set_collection), INITIALIZED);
    if (list)
    {
        size_t capacity = 0;
        list->bucket = NULL;

        _array_allocate(&(list->bucket), &capacity, list_count(list));
        //  hold reference to the last (end) item so we can calculate count
        list->end = (handle)(list->bucket);
        list->capacity = capacity;

        /*  -- debugging checks
            handle *cap = _array_get_cap_loc(list->bucket, 0);
            printf("%p ... %p ... %p (%ld)\n", list->bucket, (object)list->end, cap, list_capacity(list));
         */
    }

    return list;
}
/// @brief dispose of the current collection
/// @param list     the object to dispose (free)
void list_dispose(collection list)
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
size_t list_count(collection list)
{
    handle memDiff = list->end - (handle)list->bucket;

    return memDiff / HANDLE_SIZE;
}
/// @brief re-calculates collection capacity
/// @param list current collection
/// @return collection capacity
size_t list_capacity(collection list)
{
    handle *cap = _array_get_cap_loc(list->bucket, list_count(list));
    list->capacity = (--cap) - list->bucket;

    return list->capacity;
}
/// @brief add element to collection
/// @param list current collection
/// @param item item to add
/// @return object being added to the collection
object list_add_item(collection list, object item)
{
    size_t count = list_count(list);

    if (_array_at_threshold(count, list->capacity))
    {
        _array_resize(&(list->bucket));
    }

    (*(list->bucket + count)) = (handle)item;
    list->end += HANDLE_SIZE;

    // #if DIAG == 1
    printf("(%p ... %p ... %p) <- [%p (%ld])\n", list->bucket, (list->bucket + count), (object)(list->end), item, (handle)item);
    // #endif

    return item;
}
object list_remove_item(iterator query, object item)
{
    handle *element = NULL;
    object pElement = NULL;
    bool retFlag = false;

    while (!retFlag && Enumerator.next(query->enumer))
    {
        if (query->compare(item, query->enumer->current))
        {
            element = query->enumer->element;
            retFlag = true;
        }
    }

    if (retFlag)
    {
        pElement = (object)(*element);
        collection list = query->enumer->list;
        handle end = _array_remove_at(list->bucket, element - list->bucket);

        list->end = end;
    }

    //  we are nice enough to reset the enumerator here
    Enumerator.reset(query->enumer);

    return pElement;
}
/// @brief create a collection enumerator
/// @param list current collection
/// @return enumerator
enumerator list_get_enumerator(collection list)
{
    enumerator pEnumerator = Allocator.alloc(sizeof(struct set_enumerator), UNINITIALIZED);
    pEnumerator->list = list;
    pEnumerator->element = NULL;
    pEnumerator->current = NULL;
}
/// @brief creates a queryable iterator on the collection
/// @param list current collection
/// @param pIterator (out) iterator
/// @return TRUE if okay; otherwise FALSE
bool list_get_queryable(collection list, iterator *pIterator)
{
    enumerator pEnumerator = list_get_enumerator(list);
    if (pEnumerator)
    {
        (*pIterator) = Allocator.alloc(sizeof(struct query_iterator), UNINITIALIZED);

        if ((*pIterator))
        {
            (*pIterator)->enumer = pEnumerator;
        }
    }

    return ((*pIterator) != NULL);
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

    // #if DIAG
    // printf("move next --> %p\n", current);
    // #endif

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

/// @brief dispose of the query iterator
/// @param pIterator current iterator
void iter_dispose(iterator pIterator)
{
    enumer_dispose(pIterator->enumer);
    pIterator->enumer = NULL;
    Allocator.dealloc(pIterator);
    pIterator = NULL;
}

const struct ICollection Collection = {
    .new = &list_new,
    .dispose = &list_dispose,
    .count = &list_count,
    .capacity = &list_capacity,
    .add = &list_add_item,
    .remove = &list_remove_item,
    .get_enumerator = &list_get_enumerator,
    .get_queryable = &list_get_queryable,
};
const struct IEnumerator Enumerator = {
    .next = &enumer_move_next,
    .reset = &enumer_reset,
    .dispose = &enumer_dispose,
};
const struct IIterator Iterator = {
    .dispose = &iter_dispose,
};
