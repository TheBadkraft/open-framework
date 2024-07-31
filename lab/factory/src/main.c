#include <stdio.h>
#include <stdbool.h>

#include "../include/registry.h"
#include "../include/machine.h"

int main()
{
    printf("Registry: %d\n", MachineRegistry.count);
    printf("Factory: %s\n", MachineFactory.name);
    // BUILD_MACHINE(mtest);
}