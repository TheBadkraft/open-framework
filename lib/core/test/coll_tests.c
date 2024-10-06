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
        write_header("Test Core Collection interface");

        TEST(_collection_new);
        TEST(_collection_add);
        TEST(_collection_add_multiple);
    }
    END_SET(collection);

    BEGIN_SET(enumerator, true)
    {
        write_header("Test Core Enumerator interface");

        TEST(_collection_get_enumerator);
        TEST(_enumerator_move_next);
        TEST(_enumerator_reset);
    }
    END_SET(enumerator);

    BEGIN_SET(iterator, false)
    {
        write_header("Test Core Iterator interface");

        TEST(_collection_get_iterator);
    }
    END_SET(iterator);

    TEST_STATS();
}
void __output_collection_info(collection coll)
{
    writefln("(%p ... %p)", coll->list, coll->last);

    handle memDiff = coll->last - (handle)coll->list;
    writefln("mem Diff: %ld", memDiff);
    writefln("count: %d", memDiff / sizeof(handle));
}
void __output_collection_data(collection coll)
{
    //  first glimpse into the ease of enumerating this collection
    writeln("collection data:");
    writeln("---------------------------------------");
    //  grab copy of list pointer casting to the expected type
    handle *hPtr = coll->list;
    while (hPtr != (handle *)(coll->last))
    {
        int i = hPtr - coll->list;
        person *pData = (person *)(*hPtr);
        writefln("[%d] (%p : %p) person: [%d] %s", i, hPtr, (object)coll->last, pData->id, pData->name);

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
    collection coll = Collection.new(0);
    int i = 0;
    while (i < count)
    {
        person *p = malloc(sizeof(person));
        p->id = i;
        p->name = NAMES[i];
        Collection.add(coll, p);

        ++i;
    }

    return coll;
}

//  test cases
void _collection_new(void)
{
    writeln("Collection.new: new collection with 0 capacity");
    //  x < `DEFAULT_ARRAY_SIZE` will result in the default capacity(64)

    collection coll = Collection.new(0);
    assert(coll->capacity == DEFAULT_ARRAY_SIZE);
    assert(Collection.count(coll) == 0);
    assert(coll->list != NULL);

    __output_collection_info(coll);
    Collection.dispose(coll);
}
void _collection_add(void)
{
    writeln("Collection.add: add element to collection");

    collection coll = Collection.new(0);
    person *pExp = malloc(sizeof(person));
    pExp->id = 1;
    pExp->name = "Harold";

    assert(Collection.add(coll, pExp));
    person *pAct = (object)coll->list[0];
    assert(Collection.count(coll) == 1);

    writefln("(%p : %p) person: [%d] %s", coll->list, pAct, pAct->id, pAct->name);

    handle memDiff = coll->last - (handle)coll->list;
    assert(memDiff == sizeof(handle));

    __output_collection_info(coll);
    __output_collection_data(coll);
    Collection.dispose(coll);
    free(pExp);
}
void _collection_add_multiple(void)
{
    writeln("Collection.add: add multiple elements to collection");

    int expCount = 5;
    collection coll = __generate_collection(expCount);
    int actCount = Collection.count(coll);
    assert(actCount == expCount);

    __output_collection_info(coll);
    __output_collection_data(coll);

    //  release all generated data
    for (int i = 0; i < expCount; i++)
    {
        object d = (object)coll->list[i];
        free(d);
    }
    Collection.dispose(coll);
}
void _collection_get_enumerator(void)
{
    writeln("Collection.get_enumerator: add multiple elements to collection");

    int expCount = 5;
    collection coll = __generate_collection(expCount);

    enumerator enumer = Collection.get_enumerator(coll);
    assert(enumer != NULL);
    assert(enumer->current == NULL);
    assert(Collection.count(enumer->coll) == expCount);

    for (int i = 0; i < expCount; i++)
    {
        object d = (object)coll->list[i];
        free(d);
    }
    Collection.dispose(coll);
    Enumerator.dispose(enumer);
}
void _collection_get_iterator(void)
{
    collection coll = __generate_collection(25);

    assert(Collection.count(coll) == 25);
}

void _enumerator_move_next(void)
{
    writeln("Enumerator.next: enumerate collection accessing current object");

    int expCount = 5;
    collection coll = __generate_collection(expCount);

    enumerator enumer = Collection.get_enumerator(coll);
    int actCount = 0;
    while (Enumerator.next(enumer))
    {
        person *pData = enumer->current;
        writefln("(%p -> %p) person: [%d] %s", enumer->element, enumer->current, pData->id, pData->name);
        free(pData);

        ++actCount;
    }
    assert(actCount == expCount);
    Collection.dispose(coll);
    Enumerator.dispose(enumer);
}
void _enumerator_reset(void)
{
    writeln("Enumerator.reset: reset enumerator for re-use");

    int expCount = 5;
    collection coll = __generate_collection(expCount);

    enumerator enumer = Collection.get_enumerator(coll);
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
    Collection.dispose(coll);
    Enumerator.dispose(enumer);
}