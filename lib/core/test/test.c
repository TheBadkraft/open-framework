#include <stdio.h>
#include <stdlib.h>

#include "../open/test.h"

#include "../open/core.h"
#include "../open/io.h"
#include "../open/io_stream.h"

//  CORE: test case prototypes
void _new();
void _alloc();
void _append();
void _format();
//  IO: test case prototypes
void file_path_exists();
void dir_new();
void combine_paths();
void open_file_stream();

//	utility prototypes
void __output_string(string);
void __output_file_info(file);
void __output_directory_info(directory);
void __output_stream_info(stream);

//	test values
string pfDir = "./.data";
string pfSource = "./.data/main.C";
string pfNone = "./.data/bad.log";
string pfTemp = "./.data/test.log";

int main(int argc, char **argv)
{
    write_header("OpenPlatform: Core Unit Testing");

    BEGIN_SET(String.API, true)
    {
        // TEST(__test_not_freeable);
        TEST(_new);
        TEST(_alloc);
        TEST(_append);
        TEST(_format);
    }
    END_SET(String.API)

    BEGIN_SET(IO.API, true)
    {
        TEST(file_path_exists);
        TEST(dir_new);
        TEST(combine_paths);
        TEST(open_file_stream);
    }
    END_SET(IO.API)

    TEST_STATS();
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
void __output_file_info(file pFile)
{
    directory pDir;
    File.directory(pFile, &pDir);

    writefln("file: %s", pFile->name);
    writefln("object:    %s", B(pFile != NULL));
    writefln("created:   %s", B(File.exists(pFile)));
    writefln("directory: %s", pDir->name);
    writefln("size:      %ld", pFile->size);

    Directory.free(pDir);
}
void __output_directory_info(directory pDir)
{
    writefln("name: %s", pDir->name);
    writefln("path: %s", pDir->path);
}
void __output_stream_info(stream pStream)
{
    writefln("file:    %s", pStream->source);
    writefln("created: %s", B(Path.exists(pStream->source)));
    writefln("length:  %ld", pStream->length);
    writefln("handle:  %s", YN(pStream->fstream != NULL));
    // writefln("mode:    %s", get_mode_label(pStream->mode));
    writefln("status:  %s", (pStream->status & OK) == OK ? "OK" : "ERROR");

    string errMsg;
    Stream.get_error(pStream, &errMsg);
    writefln("error:   %s", errMsg);

    String.free(errMsg);
}

//		================================ TEST CASES ================================
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

void file_path_exists()
{
    writeln("path_exists: determine if path exists");
    assert(Utils.path_exists(pfSource));
    assert(Utils.path_exists(pfNone) == false);
}
void dir_new()
{
    directory pDir = Directory.new(pfDir);
    assert(pDir != NULL);
    assert(strcmp(".data", pDir->name) == 0);
    assert(strcmp("./.data", pDir->path) == 0);

    __output_directory_info(pDir);
    Directory.free(pDir);
}
void combine_paths()
{
    writeln("Path.combine: combine path elements with a base path");

    string expPath = "/home/david/OpenPlatform/open_framework/lib/io/.data/main.C";
    string startPath = "/home/david/OpenPlatform/open_framework/lib/io";

    string pBase;
    String.alloc(startPath, &pBase);
    writefln("strlen:  %d", strlen(startPath));
    writefln("pBase len: %d", String.length(pBase));

    Path.combine(pBase, ".data", "main.C", NULL);

    assert(strcmp(expPath, pBase) == 0);
    String.free(pBase);
}
void open_file_stream()
{
    //	gain access to a stream by opening a file
    file pFile = File.new(pfSource);
    stream pStream;

    assert(File.open(pFile, &pStream, READ));
    assert(strcmp(pfSource, pStream->source) == 0);
    assert(pStream->fstream != NULL);
    assert(Stream.is_open(pStream));

    __output_stream_info(pStream);
    Stream.free(pStream);
}