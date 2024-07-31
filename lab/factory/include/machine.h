#ifndef MACHINE_H
#define MACHINE_H

#include <stdio.h>

#include "op/data_types.h"

#include "factory.h"

static SHORT mId = 0;

enum MachineState
{
    M_IDLE,
    M_RUN,
    M_LOCK
};

typedef struct
{
    /* data */
    SHORT id;
    const CHAR *name;
    BOOL enabled;
} Machine;

static Machine _make_machine(const char *name)
{
    return (Machine){mId++, name, false};
}

CREATE_FACTORY(Machine, _make_machine);

static bool _place_machine(Machine *machine)
{
    if (machine->id == -1)
    {
        return false;
    }

    return machine->enabled = true;
}
static bool _drop_machine(Machine *machine)
{
    return machine->enabled = false;
}

#define PLACE(_machine_) _place_machine(&_machine_)
#define DROP(_machine_) _drop_machine(&_machine_)

#endif //  MACHINE_H