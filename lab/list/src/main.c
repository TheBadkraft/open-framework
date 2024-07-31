#include <stdio.h>

#include "op/data_types.h"

#include "../include/collections.h"
#include "../include/person.h"

int main(void)
{
    // struct Object object = Object.new();
    // printf("base object {%s}\n", object.guid);

    struct Collection array = ARRAY(sizeof(Person));
    printf("array [el.size:%d  count:%d]\n", array.elSize, array.count);

    Person p1 = {
        .fname = "David",
        .lname = "Smith"};
    int ndx = APPEND(array, p1);

    if (ndx == -1)
    {
        printf("[%d] error pushing item\n", ndx);
    }
    else
    {
        printf("array[%d] [el.size:%d  count:%d]\n", ndx, array.elSize, array.count);
    }

    // Person p2 = {
    //     .fname = "Mandy",
    //     .lname = "Smith"};
    // ndx = PUSH(array, p2);

    // if (ndx == -1)
    // {
    //     printf("[%d] error adding item\n", ndx);
    // }
    // else
    // {
    //     printf("array [el.size:%d  count:%d]\n", array.elSize, array.count);
    // }

    ndx = 0;
    Person *p3 = GET(array, Person, ndx);
    if (p3 != NULL)
    {
        printf("person [%d] %s %s\n", ndx, p3->fname, p3->lname);
    }
}