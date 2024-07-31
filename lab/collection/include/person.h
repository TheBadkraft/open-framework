#ifndef _PERSON_H
#define _PERSON_H

#include <stdbool.h>

#include "cstring.h"

static const char *type_name = "person";

typedef struct person_t
{
    string *fname;
    string *lname;
} person;

extern const struct Person_t
{
    person *(*new)(const char *fname, const char *lname);
    void (*destroy)(person *p);
} Person;

typedef void *(*iterator)(person *p);

void free_person(person *p);

extern const struct People_c
{
    bool (*is)();
    size_t (*count)();
    size_t (*capacity)();
    void (*alloc)(int capacity);
    size_t (*init)();
    person *(*append)(person *p);
    void (*destroy)(void (*itfunc)(person *p));
} People;

#endif //  _PERSON_H