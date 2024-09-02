#include <stdio.h>
#include <stdlib.h>

#include "../open/test.h"

#include "../open/core.h"
#include "../open/io.h"
#include "../open/io_stream.h"

//	FILE: test case prototypes
void file_path_exists();
void file_size();
void create_file_obj();
void create_file();
void file_directory();
//	DIRECTORY: test case prototypes
void dir_new();
void dir_exists();
void dir_current();
//	PATH: test case prototypes
void get_abs_path();
void combine_paths();
void path_directory();
void path_file_directory();
//	STREAM: test case prototypes
void get_stream_error();
void new_stream();
void open_file_stream();

//	utility prototypes
void __output_file_info(file);
void __output_directory_info(directory);
void __output_stream_info(stream);

//	test values
string pfDir = "./.data";
string pfSource = "./.data/main.C";
string pfNone = "./.data/bad.log";
string pfTemp = "./.data/test.log";

int main(int argc, string *argv)
{
	if (Utils.path_exists(pfTemp))
	{
		remove(pfTemp);
	}

	BEGIN_SET(file, true)
	if (doTests)
		write_header("OP Tests: IO.File");
	TEST(file_path_exists);
	TEST(file_size);
	TEST(create_file_obj);
	TEST(create_file);
	TEST(file_directory);
	END_SET(file)

	BEGIN_SET(directory, true)
	if (doTests)
		write_header("OP Tests: IO.Directory");
	TEST(dir_current);
	TEST(dir_new);
	TEST(dir_exists);
	END_SET(directory)

	BEGIN_SET(Path, true)
	if (doTests)
		write_header("OP Tests: IO.Path");
	TEST(get_abs_path);
	TEST(combine_paths);
	TEST(path_directory);
	TEST(path_file_directory);
	END_SET(Path)

	BEGIN_SET(stream, true)
	if (doTests)
		write_header("OP Tests: IO.Stream");
	TEST(get_stream_error);
	TEST(new_stream);
	TEST(open_file_stream);
	END_SET(stream)

	TEST_STATS();
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
void file_path_exists()
{
	writeln("path_exists: determine if path exists");
	assert(Utils.path_exists(pfSource));
	assert(Utils.path_exists(pfNone) == false);
}
void file_size()
{
	writeln("File.size: get file size");

	file pFile = File.new(pfNone);
	File.size(pFile);

	assert(pFile->size == -1);

	free(pFile);
}
void create_file_obj()
{
	writeln("File.new: new file object");

	file pFile = File.new(pfTemp);
	assert(pFile != NULL);
	assert(pFile->size == -1);
	assert(File.exists(pFile) == false);

	__output_file_info(pFile);
	if (File.exists(pFile))
	{
		writefln("deleting %s", pFile->name);
		File.delete(pFile);
	}

	free(pFile);
}
void create_file()
{
	writeln("File.create: create new file");

	file pFile = File.new(pfTemp);
	assert(File.create(pFile));
	assert(pFile->size == 0);

	__output_file_info(pFile);
	if (File.exists(pFile))
	{
		writefln("deleting %s", pFile->name);
		File.delete(pFile);
	}

	File.free(pFile);
}
void file_directory()
{
	writeln("File.directory: get file directory");

	directory pDir;
	file pFile;
	File.directory(pFile = File.new(pfSource), &pDir);
	assert(pDir != NULL);
	assert(strcmp(".data", pDir->name) == 0);
	assert(strcmp("./.data", pDir->path) == 0);

	__output_directory_info(pDir);
	File.free(pFile);
	Directory.free(pDir);
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
void dir_exists()
{
	directory pDir = Directory.new(pfDir);
	assert(Directory.exists(pDir));

	__output_directory_info(pDir);
	Directory.free(pDir);
}
void dir_current()
{
	directory pDir;
	Directory.current(&pDir);
	assert(pDir != NULL);

	__output_directory_info(pDir);
	Directory.free(pDir);
}

void get_abs_path()
{
	writeln("Path.absolute: get the absolute members of a relative path");

	string expPath = "/home/david/OpenPlatform/open_framework/lib/core/.data/main.C";
	string actPath; // = String.new();

	bool retOk = Path.absolute(pfSource, &actPath);
	assert(retOk);
	assert(strcmp(expPath, actPath) == 0);

	writefln("%s --> %s", pfSource, actPath);
	if (retOk)
	{
		//	if the path does not exist, actPath is never allocated
		String.free(actPath);
	}

	retOk = Path.absolute(pfNone, &actPath);
	assert(retOk == false);

	writefln("%s -> %s", pfNone, retOk ? "exits" : "does not exist");
	if (retOk)
	{
		String.free(actPath);
	}
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
void path_directory()
{
	writeln("Path.directory: return char* directory from path");

	string expDir = ".data";
	string pDir = Path.directory(pfDir);

	assert(strcmp(expDir, pDir) == 0);
}
void path_file_directory()
{
	writeln("Path.directory: return char* directory from file path");

	string expDir = ".data";
	string pDir = Path.directory(pfSource);

	assert(strcmp(expDir, pDir) == 0);
}

void get_stream_error()
{
	string expMsg = "Path not found";

	struct io_stream target = {
		.error = PATH_NOT_FOUND};

	string errMsg;
	Stream.get_error(&target, &errMsg);

	assert(strcmp(expMsg, errMsg) == 0);

	String.free(errMsg);
}
void new_stream()
{
	//	new stream
	stream pStream = Stream.new(pfNone);

	assert(pStream != NULL);
	assert(strcmp(pfNone, pStream->source) == 0);

	__output_stream_info(pStream);
	Stream.free(pStream);
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
