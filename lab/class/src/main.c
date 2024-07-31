#include <stdio.h>

#include "../include/cstring.h"
#include "../include/reftable.h"
#include "../include/class.h"
#include "../include/person.h"

int main(int argc, char *argv[])
{
	printf("ref table[%d]: %ld\n", RefTable.is(), RefTable.length());
	person *p = Person.new("John", "Doe");

	printf("ref table[%d]: %ld\n", RefTable.is(), RefTable.length());
	class_t *c = p->__class;
	if (c->ref == NULL)
	{
		return -1;
	}
	rt_entry *refEntry = c->ref;
	printf("(%ld){%s}\n", refEntry->hash, refEntry->type->buffer);

	// if (refEntry->value == NULL)
	// {
	// 	printf("null reference\n");
	// }
	// person *pRef = (person *)(c->ref->value);
	// if (pRef == NULL)
	// {
	// 	puts("reference is null\n");
	// }
}
