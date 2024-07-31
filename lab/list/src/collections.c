#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/collections.h"

static struct Collection array(int elSize)
{
    struct Object object = Object.new();
    printf("object created [%s]\n", object.guid);

    struct Collection coll = {
        .object = object,
        .elSize = elSize,
        .count = 0,
        .bucket = NULL};

    return coll;
}
static int append(struct Collection *self, void *item)
{
    void *temp = NULL;
    int len = self->count;
    int size = self->elSize;
    int ndx = -1;

    if (len == 0)
    {
        temp = malloc(size);
    }
    else
    {
        temp = realloc(self->bucket, size);
    }

    if (temp)
    {
        ndx = len++;
        self->bucket = temp;
        temp = memcpy(self->bucket + (ndx * size), item, size);
    }

    if (temp == NULL)
    {
        printf("error pushing item to array [%d]\n", ndx);
    }
    else
    {
        self->count = len;
    }

    return ndx;
}
static void *get(struct Collection *self, int index)
{
    if (index >= self->count)
    {
        printf("index outside array range [%d >= %d]", index, self->count);
        return NULL;
    }

    return self->bucket + (index * self->elSize);
}

const struct collection Collection = {
    .array = &array,
    .append = &append,
    .get = &get};
