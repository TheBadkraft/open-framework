#include "../include/class.h"
#include "../include/person.h"

INIT_CLASS(person);

static person *new_person(const char *fname, const char *lname)
{
    person *p = malloc(sizeof(person));
    init_person(p);
    p->fname = String.alloc(fname);
    p->lname = String.alloc(lname);
}

const struct Person_t Person = {
    .new = &new_person};
