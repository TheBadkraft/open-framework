#include <stdint.h>

#include "../include/person.h"

#define DEFAULT_CAPACITY 32
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

typedef struct person_collection
{
    person *coll;
    size_t count;
    size_t capacity;
} persons;
static persons *people = NULL;
static const person NULL_PERSON = {NULL, NULL};

uint64_t type_hash;

void free_person(person *p)
{
    printf("free(ing) [%s, %s]\n", p->lname->buffer, p->fname->buffer);
    String.free(p->fname);
    String.free(p->lname);

    free(p);
}

uint64_t gen_hash(const char *key)
{
    uint64_t hash = FNV_OFFSET;
    for (const char *p = key; *p; p++)
    {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}
bool expand_coll()
{
    size_t capacity = people->capacity + (DEFAULT_CAPACITY * 2);
    if (capacity < people->capacity)
    {
        //  overflow condition: table capacity too large
        return false;
    }
    //  reallocate
    people->coll = realloc(people->coll, capacity * sizeof(person));
}

static person *new_person(const char *fname, const char *lname)
{
    person *p = malloc(sizeof(person));
    p->fname = String.alloc(fname);
    p->lname = String.alloc(lname);

    // printf("person key [%s:%ld]\n", key->buffer, p->hash);

    return p;
}

static bool coll_is()
{
    bool is = people != NULL;

    return is;
}
static size_t coll_count()
{
    return coll_is() ? people->count : -1;
}
static size_t coll_capacity()
{
    return coll_is() ? people->capacity : -1;
}
static int coll_iterate(void (*itfunc)(person *p))
{
    int ndx = 0;
    while (ndx < people->count)
    {
        itfunc(people->coll + ndx);
        ++ndx;
    }

    return ndx;
}
static void coll_alloc(int capacity)
{
    people = malloc(sizeof(people));
    people->capacity = capacity;
    people->count = 0;
    people->coll = calloc(people->capacity, sizeof(person));

    //  calc base type hash
    type_hash = gen_hash(type_name);
}
static size_t coll_init()
{
    coll_alloc(DEFAULT_CAPACITY);
    return (people->capacity);
}
static person *coll_append_item(person *p)
{
    if (people->count >= people->capacity * .75)
    {
        expand_coll();
    }
    people->coll[people->count++] = *p;
    return p;
}
static void coll_destroy(void (*itfunc)(person *p))
{
    //  1. iterate items and free
    int count = 0;
    if (itfunc != NULL)
    {
        count = coll_iterate(itfunc);
    }
    printf("free(ed) %d item%s\n", count, count > 1 ? "s" : "");
    // free(people->coll);
    //  2. free collection
    // free(people);
}

const struct Person_t Person = {
    .new = &new_person,
    .destroy = &free_person};

const struct People_c People = {
    .is = &coll_is,
    .count = &coll_count,
    .capacity = &coll_capacity,
    .init = &coll_init,
    .alloc = &coll_alloc,
    .append = &coll_append_item,
    .destroy = &coll_destroy};
