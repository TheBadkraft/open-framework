#include "../../open/core/core.h"

jmp_buf jump;

static void segv(int);

bool __is_allocated(void *);

static void segv(int sig)
{
    longjmp(jump, 1);
}

/*
    Determine if a variable has been allocated (malloc'd)
*/
bool __is_allocated(void *x)
{
    volatile char c;
    bool isAlloc = true;

    if (x == NULL)
    {
        isAlloc = false;
        goto quit;
    }

    signal(SIGSEGV, segv);

    if (!setjmp(jump))
    {
        c = *(char *)(x);
    }
    else
    {
        isAlloc = false;
    }
    signal(SIGSEGV, SIG_DFL);

quit:
    return (isAlloc);
}

const struct O_Core Core = {
    .is_allocated = &__is_allocated};
