#ifndef COLLECTIONS_H
#define COLLECTIONS_H

#include "object.h"

struct Collection
{
    struct Object object;
    int elSize;
    int count;
    void *bucket;
};

typedef struct Collection (*coll)(int elSize);
typedef int (*append_item)(struct Collection *self, void *item);
typedef void *(*get_item)(struct Collection *self, int ndx);

extern const struct collection
{
    coll array;
    append_item append;
    get_item get;
} Collection;

#define ARRAY(_collType_) Collection.array(sizeof(_collType_))
#define APPEND(_coll_, _item_) Collection.append(&_coll_, &_item_)
#define GET(_coll_, _collType_, _index_) (_collType_ *)Collection.get(&_coll_, _index_)

#endif //  COLLECTIONS_H