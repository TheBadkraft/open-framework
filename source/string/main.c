#include <assert.h>

#include "../../source/open/core.h"

void print_header(char *);
void output_string(string *);

int main(int argc, char **argv)
{
    print_header("String.new");
    string *str = String.new();
    assert(str != NULL);

    char *val = "this is a string";
    String.append(str, val);
    assert(str->buffer != NULL);
    assert(strlen(str->buffer) == strlen(val));

    output_string(str);
    free(str);

    //  =========================================================
    print_header("String.alloc");
    char *hello = "Hello";
    char *world = "World";
    str = String.alloc(hello);
    assert(str != NULL);
    assert(String.length(str) == strlen(hello));
    String.writeln(str);
    String.append(str, world);
    assert(String.length(str) == strlen(hello) + strlen(world));

    output_string(str);
    free(str);

    //  =========================================================
    print_header("String.format");
    str = String.format("%s, %s", hello, world);
    assert(String.length(str) == strlen("Hello, World"));

    output_string(str);
    free(str);

    //  =========================================================
    print_header("String.appendf");
    str = String.alloc(hello);
    String.appednf(str, ", %s!", world);
    output_string(str);
    free(str);

    //  =========================================================
    print_header("end open_string_tests ...");
}

void print_header(char *header)
{
    printf("%s\n", header);
}
void output_string(string *str)
{
    String.writeln(str);
    printf("str len (%ld)\n", strlen(str->buffer));
    printf("str cap (%ld)\n", str->capacity);
    printf("\n");
}