#include<stdio.h>

//  defines
#define TRUE 1
#define FALSE !(TRUE)

/*
    argc: number of args
    argv: arg array
*/
int arg_test(int argc, char *argv[]) {
    //  output number of args
    printf("arg count: %d\n", argc);
    printf("%s\n", *argv);

    return TRUE;
}