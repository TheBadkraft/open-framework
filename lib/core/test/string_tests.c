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
void _append();
void _format();
void _join_strings();
void _alloc_join();

//  utility prototypes
void __output_string(string);

int main(int argc, char **argv)
{
    write_header("OpenPlatform: Core Unit Testing");
    Allocator.init();

    BEGIN_SET(String.API, true)
    {
        // TEST(__test_not_freeable);
        TEST(_is_empty);
        TEST(_is_noe);
        TEST(_new);
        TEST(_alloc);
        TEST(_append);
        TEST(_format);
        TEST(_join_strings);
        TEST(_alloc_join);
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
    writeln("Path.combine: combine path elements with a base path");

    string expPath = "/home/david/OpenPlatform/open_framework/lib/io/.data/main.C";
    string root = "/home/david/OpenPlatform/open_framework/lib/io";

    string pBase;
    // String.new(0, &pBase);
    writefln("root:  %d", strlen(root));

    String.join("/", &pBase, root, ".data", "main.C", NULL);

    assert(strcmp(expPath, pBase) == 0);
    __output_string(pBase);

    String.free(pBase);
}
void _alloc_join()
{
    static const string __DEF_PATH = ".data/sigmac.def";
    string expPath = "c:/home/david/OpenPlatform/open_framework/lib/core/.data/sigmac.def";
    //	get the current working directory
    string cwdir;
    String.alloc("c:/home/david/OpenPlatform/open_framework/lib/core", &cwdir);

    string def_path;
    String.new(0, &def_path);

    String.join("/", &def_path, cwdir, __DEF_PATH, NULL);

    assert(strcmp(expPath, def_path) == 0);
    __output_string(def_path);

    String.free(cwdir);
    String.free(def_path);
}