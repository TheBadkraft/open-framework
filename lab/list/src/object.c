#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

#include "../include/object.h"

void gen_uuid(char guid[UUID_STR_LEN])
{
    uuid_t b;
    uuid_generate(b);
    uuid_unparse_lower(b, guid);
}

static struct Object new()
{
    char guid[UUID_STR_LEN];
    gen_uuid(guid);

    return (struct Object){.guid = guid};
}
const struct Class Object = {.new = &new};