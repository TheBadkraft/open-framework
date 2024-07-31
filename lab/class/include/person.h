#ifndef _PERSON_H
#define _PERSON_H

#include "class.h"
#include "cstring.h"

BEGIN_CLASS(person)
string *fname;
string *lname;
END_CLASS(person)

extern const struct Person_t
{
    person *(*new)(const char *fname, const char *lname);
} Person;

#endif //  _PERSON_H