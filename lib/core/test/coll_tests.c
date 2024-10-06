#include <stdio.h>
#include <stdlib.h>

#include "../open/testing/test.h"

#include "../open/core/core.h"
#include "../open/collections/collection.h"

static string *NAMES = (string[]){
    "Sam", "Harold", "Betty", "James", "Jill", "Sharon", "Jackson", "Tully", "Sean", "Debra",
    "Walter", "Don", "Frank", "Mary", "Cathy", "Martin", "Jesse", "Karly", "Daniel", "Billy",
    "Casey", "Todd", "Anna", "Keith", "Hugo", "Jason", "Kumar", "Jennifer", "Allison", "Andy"};

typedef struct tst_person
{
    int id;
    string name;
} person;

//  test case prototypes
void _collection_new(void);
void _collection_add(void);
void _collection_add_multiple(void);
void _collection_get_enumerator(void);
void _collection_get_iterator(void);

void _enumerator_move_next(void);
void _enumerator_reset(void);

//  utility prototypes
void __output_collection_info(collection);
void __output_collection_data(collection);
void __output_enumerator_data(enumerator);
void __dispose_enumerator_data(enumerator);
collection __generate_collection(int);

int main(int argc, string *argv)
{
    BEGIN_SET(collection, true)
    {
        write_header("Collection interface");

        TEST(_collection_new);
        TEST(_collection_add);
        TEST(_collection_add_multiple);
    }
    END_SET(collection);

    BEGIN_SET(enumerator, true)
    {
        write_header("Enumerator interface");

        TEST(_collection_get_enumerator);
        TEST(_enumerator_move_next);
        TEST(_enumerator_reset);
    }
    END_SET(enumerator);

    BEGIN_SET(iterator, false)
    {
        write_header("Iterator interface");

        TEST(_collection_get_iterator);
    }
    END_SET(iterator);

    TEST_STATS();
}
void __output_collection_info(collection list)
{
    writefln("(%p ... %p)", list->bucket, list->end);

    handle memDiff = list->end - (handle)list->bucket;
    writefln("mem Diff: %ld", memDiff);
    writefln("count: %d", memDiff / sizeof(handle));
}
void __output_collection_data(collection list)
{
    //  first glimpse into the ease of enumerating this collection
    writeln("collection data:");
    writeln("---------------------------------------");
    //  grab copy of list pointer casting to the expected type
    handle *hPtr = list->bucket;
    while (hPtr != (handle *)(list->end))
    {
        int i = hPtr - list->bucket;
        person *pData = (person *)(*hPtr);
        writefln("[%d] (%p : %p) person: [%d] %s", i, hPtr, (object)list->end, pData->id, pData->name);

        ++hPtr;
    }

    writeln("=======================================");
}
void __output_enumerator_data(enumerator enumer)
{
    int i = 0;
    while (Enumerator.next(enumer))
    {
        person *pData = enumer->current;
        writefln("[%d] (%p) person: [%d] %s", i, enumer->current, pData->id, pData->name);

        ++i;
    }
}
void __dispose_enumerator_data(enumerator enumer)
{
    while (Enumerator.next(enumer))
    {
        free(enumer->current);
    }
}
collection __generate_collection(int count)
{
    collection list = Collection.new();
    int i = 0;
    while (i < count)
    {
        person *p = malloc(sizeof(person));
        p->id = i;
        p->name = NAMES[i];
        Collection.add(list, p);

        ++i;
    }

    return list;
}

//  test cases
void _collection_new(void)
{
    writeln("Collection.new: new collection with default capacity");
    //  x < `DEFAULT_ARRAY_SIZE` will result in the default capacity(64)

    collection list = Collection.new();
    //  NOTE: calling `Collection.capacity(...) re-calculate the capacity
    assert(Collection.capacity(list) == DEFAULT_ARRAY_SIZE);
    assert(Collection.count(list) == 0);
    assert(list->bucket != NULL);

    __output_collection_info(list);
    Collection.dispose(list);
}
void _collection_add(void)
{
    writeln("Collection.add: add element to collection");

    collection list = Collection.new();
    person *pExp = malloc(sizeof(person));
    pExp->id = 1;
    pExp->name = "Harold";

    assert(Collection.add(list, pExp));
    person *pAct = (object)list->bucket[0];
    assert(Collection.count(list) == 1);

    writefln("(%p : %p) person: [%d] %s", list->bucket, pAct, pAct->id, pAct->name);

    handle memDiff = list->end - (handle)list->bucket;
    assert(memDiff == sizeof(handle));

    __output_collection_info(list);
    __output_collection_data(list);
    Collection.dispose(list);
    free(pExp);
}
void _collection_add_multiple(void)
{
    writeln("Collection.add: add multiple elements to collection");

    int expCount = 5;
    collection list = __generate_collection(expCount);
    int actCount = Collection.count(list);
    assert(actCount == expCount);

    __output_collection_info(list);
    __output_collection_data(list);

    //  release all generated data
    for (int i = 0; i < expCount; i++)
    {
        object d = (object)list->bucket[i];
        free(d);
    }
    Collection.dispose(list);
}
void _collection_get_enumerator(void)
{
    writeln("Collection.get_enumerator: add multiple elements to collection");

    int expCount = 5;
    collection list = __generate_collection(expCount);

    enumerator enumer = Collection.get_enumerator(list);
    assert(enumer != NULL);
    assert(enumer->current == NULL);
    assert(Collection.count(enumer->list) == expCount);

    for (int i = 0; i < expCount; i++)
    {
        object d = (object)list->bucket[i];
        free(d);
    }
    Collection.dispose(list);
    Enumerator.dispose(enumer);
}
void _collection_get_iterator(void)
{
    collection list = __generate_collection(25);

    assert(Collection.count(list) == 25);
}

void _enumerator_move_next(void)
{
    writeln("Enumerator.next: enumerate collection accessing current object");

    int expCount = 5;
    collection list = __generate_collection(expCount);

    enumerator enumer = Collection.get_enumerator(list);
    int actCount = 0;
    while (Enumerator.next(enumer))
    {
        person *pData = enumer->current;
        writefln("(%p -> %p) person: [%d] %s", enumer->element, enumer->current, pData->id, pData->name);
        free(pData);

        ++actCount;
    }
    assert(actCount == expCount);
    Collection.dispose(list);
    Enumerator.dispose(enumer);
}
void _enumerator_reset(void)
{
    writeln("Enumerator.reset: reset enumerator for re-use");

    int expCount = 5;
    collection list = __generate_collection(expCount);

    enumerator enumer = Collection.get_enumerator(list);
    int actCount = 0;
    while (Enumerator.next(enumer))
    {
        ++actCount;
    }
    writefln("enumerated records: %d", actCount);
    Enumerator.reset(enumer);
    assert(enumer->current == NULL);
    assert(enumer->element == NULL);

    __output_enumerator_data(enumer);
    Enumerator.reset(enumer);
    __dispose_enumerator_data(enumer);
    Collection.dispose(list);
    Enumerator.dispose(enumer);
}