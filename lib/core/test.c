#include <stdio.h>

#include "../../testing/test.h"
#include "../open/core.h"

void manipulate_strings();

int main(int argc, char **argv)
{
    printf("OpenPlatform: Core\n");

    TEST(manipulate_strings);
}

//  TEST CASES
void manipulate_strings()
{
    assert(true);
}