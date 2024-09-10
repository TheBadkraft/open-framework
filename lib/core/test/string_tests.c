#include <stdio.h>

#include "../open/test.h"
#include "../open/core.h"
#include "../open/allocator.h"

//  test case prototypes
void _not_freeable();
void _is_empty();
void _is_noe();
void _new();
void _alloc();
void _copy();
void _resize();
void _truncate();
void _append();
void _format();
void _join_strings();
void _join_with_const_dest();

//  utility prototypes
void __output_string(string);

int main(int argc, char **argv)
{
    write_header("OpenPlatform: Core Unit Testing");
    BEGIN_SET(String.API, true)
    {
        // TEST(__test_not_freeable);
        // TEST(_is_empty);
        // TEST(_is_noe);
        // TEST(_new);
        // TEST(_alloc);
        TEST(_copy);
        TEST(_resize);
        TEST(_truncate);
        // TEST(_append);
        // TEST(_format);
        // TEST(_join_strings);
        // TEST(_join_with_const_dest);
    }
    END_SET(String.API)

    TEST_STATS();
    Allocator.terminate();
}

void __output_string(string text)
{
    string msg = NULL;
    if (text == NULL)
    {
        msg = "NULL";
    }
    else if (String.is_empty(text))
    {
        msg = "EMPTY";
    }
    writefln("text:     %s", msg != NULL ? msg : text);
    writefln("text len:  %ld", String.length(text));
    writefln("freeable: %s", YN(String.freeable(text)));
}

//  TEST CASES
void _not_freeable()
{
    //  String.freeable(...) not working yet
    return;
    //   a 'non-string' is an unallocated char*
    string text = "hello";
    assert(!String.freeable(text));
}
void _is_empty()
{
    string text = String.empty;
    assert(String.is_empty(text));

    __output_string(text);

    //  was not malloc'd
    // String.free(text);
}
void _is_noe()
{
    string text = NULL;
    assert(String.is_null_or_empty(text));

    __output_string(text);
    String.free(text);
}
void _new()
{
    string text;
    String.new(0, &text);
    assert(text != NULL);

    __output_string(text);
    String.free(text);
}
void _alloc()
{
    string text;
    char *hello = "Hello";

    String.alloc(hello, &text);

    assert(strcmp(hello, text) == 0);

    __output_string(text);
    String.free(text);
}
void _copy()
{
    writeln("String.copy: copies string const into malloc'd & sized string");
    string expStr = "Original String";

    string dest;
    //  unlike alloc which takes an uninitialized string, copy expects a properly sized &
    //  initialized string
    String.new(String.length(expStr) + 1, &dest);
    String.copy(dest, expStr);

    assert(strcmp(expStr, dest) == 0);
    __output_string(dest);

    String.free(dest);
}
void _resize()
{
    writeln("String.resize: resize an initialized string");
    string expStr = "Hello, World";
    string pt1 = "Hello";
    string pt2 = "World";

    string dest;
    //  allocate with initial value
    String.alloc(pt1, &dest);
    String.resize(String.length(dest) + String.length(pt2) + 2, &dest);

    //  copy_to will not fault
    String.copy_to(", ", dest, 5);
    String.copy_to(pt2, dest, 7);

    assert(strcmp(expStr, dest) == 0);
    __output_string(dest);

    String.free(dest);
}
void _truncate()
{
    writeln("String.truncate: truncate a string");
    string expStr = "Hello,";
    string target;
    String.alloc("Hello, World", &target);

    String.truncate(6, &target);

    assert(strcmp(expStr, target) == 0);
    __output_string(target);

    String.free(target);
}
void _append()
{
    string hello = "Hello";
    string world = "World";
    string text;

    string expStr = "HelloWorld";
    size_t expLen = strlen(expStr);

    String.alloc(hello, &text);
    String.append(text, world);
    assert(strcmp(expStr, text) == 0);
    assert(String.length(text) == expLen);

    __output_string(text);
    String.free(text);
}
void _format()
{
    string hello = "Hello";
    string world = "World";
    string text;

    string expStr = "Hello, World";
    size_t expLen = strlen(expStr);

    String.new(0, &text);
    String.format("%s, %s", &text, hello, world);
    assert(strcmp(expStr, text) == 0);
    assert(String.length(text) == expLen);

    __output_string(text);
    String.free(text);
}
void _join_strings()
{
    writeln("Path.combine: combine path elements with empty base path");

    string expPath = "/home/david/OpenPlatform/open_framework/lib/io/.data/main.C";
    string root = "/home/david/OpenPlatform/open_framework/lib/io";

    //  destination needs to be malloc'd and sized
    string pBase;
    //  expected pathLen - do not pass the destination in the join_len
    int pathLen = String.join_len("/", root, ".data", "main.C", NULL);
    String.new(pathLen, &pBase);
    // String.new(0, &pBase);
    writefln("root:  %d", strlen(root));

    String.join("/", &pBase, root, ".data", "main.C", NULL);

    assert(strcmp(expPath, pBase) == 0);
    __output_string(pBase);

    String.free(pBase);
}
void _join_with_const_dest()
{
    writeln("Path.combine: combine path elements with populated base path");

    string expPath = "/home/david/OpenPlatform/open_framework/lib/io/.data/main.C";
    string pathPart = "open_framework/lib/io";
    string pathData = ".data";
    string pathMainC = "main.C";

    //  root is the destination
    string pathRoot = "/home/david/OpenPlatform";

    int pathLen = String.join_len("/", pathPart, pathData, pathMainC, NULL);
    //  then add the destination len if not empty
    pathLen += String.length(pathRoot);

    //  this will throw a seg fault because pathRoot is not malloc'd
    // String.join("/", &pathRoot, pathPart, pathData, pathMainC, NULL);
    //  must allocate the string first and size properly to the expected pathLen
    String.alloc(pathRoot, &pathRoot);
    String.resize(pathLen, &pathRoot);
    /*
    //  there are a couple of ways to do the above ...

        Example:
        String.new(pathLen, &pathRoot);
        String.copy(pathRoot, "/home/david/OpenPlatform");
    */
    String.join("/", &pathRoot, pathPart, pathData, pathMainC, NULL);

    assert(strcmp(expPath, pathRoot) == 0);
    __output_string(pathRoot);

    String.free(pathRoot);
}
