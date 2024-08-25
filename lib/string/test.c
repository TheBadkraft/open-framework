#include <assert.h>

#include "../../testing/test.h"

#include "../open/core.h"

//  utility prototypes
void output_string(string *);
//  test case prototypes
void create_new_string();
void allocate_new_string();
void append_string();
void format_string();
void append_format_string();

int main(int argc, char **argv)
{
    write_header("OP.Tests: String");

    TEST(create_new_string);
    TEST(allocate_new_string);
    TEST(append_string);
    TEST(format_string);
}

void create_new_string()
{
    string *str = String.new();
    assert(str != NULL);
    assert(str->buffer == NULL);
    assert(str->capacity == 0);

    output_string(str);

    //  TODO: what is causing String.free(..) to seg fault?
    // free(str);

    String.free(str);
}
void allocate_new_string()
{
    char *expStr = "this is a string";
    size_t expLen = strlen(expStr);

    string *str = String.alloc(expStr);
    writefln("expStr '%s' ('%ld')", expStr, strlen(expStr), NULL);

    assert(str->buffer != NULL);
    assert(strlen(str->buffer) == strlen(expStr));
    assert(str->capacity == strlen(expStr) + 1);

    output_string(str);
    String.free(str);
}
void append_string()
{
    char *hello = "Hello";
    char *world = "World";

    string *str = String.alloc(hello);
    String.append(str, world);
    assert(String.length(str) == strlen(hello) + strlen(world));

    output_string(str);
    String.free(str);
}
void format_string()
{
    char *hello = "Hello";
    char *world = "World";

    string *str = String.new();
    str = String.format("%s, %s", hello, world);
    assert(String.length(str) == strlen("Hello, World"));

    output_string(str);
    String.free(str);
}
void append_format_string()
{
    char *hello = "Hello";
    char *world = "World";

    string *str = String.alloc(hello);
    String.appendf(str, ", %s!", world);
    assert(String.length(str) == strlen("Hello, World"));

    output_string(str);
    String.free(str);
}

void output_string(string *str)
{
    String.writeln(str);
    printf("str len (%ld)\n", str->buffer != NULL ? strlen(str->buffer) : 0);
    printf("str cap (%ld)\n", str->capacity);
}
