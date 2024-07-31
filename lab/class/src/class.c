#include <stdio.h>

#include "../include/class.h"

static class_t *class_new(const char *type_name, void *t)
{
    reftbl *table = rt();
    rt_entry *entry = table->ref(type_name, t);

    class_t *cls = malloc(sizeof(class_t));
    cls->ref = entry;

    return cls;
}

const struct Class_t Class = {
    .new = &class_new};