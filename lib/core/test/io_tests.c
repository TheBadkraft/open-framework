#include <stdio.h>
#include <stdlib.h>

#include "../open/test.h"

#include "../open/core.h"
#include "../open/io.h"
#include "../open/io_stream.h"
#include "../open/allocator.h"

//	FILE: test case prototypes
void _file_path_exists();
void _file_size();
void _create_file_obj();
void _new_file_from_path();
void _create_file();
void _file_directory();
//	DIRECTORY: test case prototypes
void _dir_new();
void _dir_exists();
void _dir_current();
//	PATH: test case prototypes
void _get_abs_path();
void _combine_paths();
void _combine_with_empty_base();
void _path_directory();
void _path_file_directory();
//	STREAM: test case prototypes
void _get_stream_error();
void _new_stream();
void _open_file_stream();

//	utility prototypes
void _output_file_info(file);
void _output_directory_info(directory);
void _output_stream_info(stream);

//	target subject
file def_file = NULL;

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
	{
		if (doTests)
			write_header("OP Tests: IO.File");
		TEST(_file_path_exists);
		TEST(_file_size);
		TEST(_create_file_obj);
		TEST(_create_file);
		TEST(_new_file_from_path);
		TEST(_file_directory);
	}
	END_SET(file)

	BEGIN_SET(directory, true)
	{
		if (doTests)
			write_header("OP Tests: IO.Directory");
		TEST(_dir_current);
		TEST(_dir_new);
		TEST(_dir_exists);

		if (doTests)
			Allocator.flush();
	}
	END_SET(directory)

	BEGIN_SET(Path, true)
	{
		if (doTests)
			write_header("OP Tests: IO.Path");
		// TEST(_get_abs_path);
		TEST(_combine_paths);
		TEST(_combine_with_empty_base);
		TEST(_path_directory);
		TEST(_path_file_directory);

		if (doTests)
			Allocator.flush();
	}
	END_SET(Path)

	BEGIN_SET(stream, true)
	{
		if (doTests)
			write_header("OP Tests: IO.Stream");
		TEST(_get_stream_error);
		TEST(_new_stream);
		TEST(_open_file_stream);

		if (doTests)
			Allocator.flush();
	}
	END_SET(stream)

	TEST_STATS();

	Allocator.terminate();
}

void _output_file_info(file pFile)
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
void _output_directory_info(directory pDir)
{
	writefln("name: %s", pDir->name);
	writefln("path: %s", pDir->path);
}
void _output_stream_info(stream pStream)
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
void _file_path_exists()
{
	writeln("path_exists: determine if path exists");
	assert(Utils.path_exists(pfSource));
	assert(Utils.path_exists(pfNone) == false);
}
void _file_size()
{
	writeln("File.size: get file size");

	file pFile = File.new(pfNone);
	File.size(pFile);

	assert(pFile->size == -1);

	File.free(pFile);
	Allocator.flush();
}
void _create_file_obj()
{
	writeln("File.new: new file object");

	file pFile = File.new(pfTemp);
	assert(pFile != NULL);
	assert(pFile->size == -1);
	assert(File.exists(pFile) == false);

	_output_file_info(pFile);
	if (File.exists(pFile))
	{
		writefln("deleting %s", pFile->name);
		File.delete(pFile);
	}

	File.free(pFile);
	Allocator.flush();
}
void _create_file()
{
	writeln("File.create: create new file");

	file pFile = File.new(pfTemp);
	assert(File.create(pFile));
	assert(pFile->size == 0);

	_output_file_info(pFile);
	if (File.exists(pFile))
	{
		writefln("deleting %s", pFile->name);
		File.delete(pFile);
	}

	File.free(pFile);
}
void _new_file_from_path()
{
	/*
		Duplicating conditions causing 'realloc(): invalid next size'
	*/
	static const string __DEF_PATH = ".data/sigmac.def";

	//	get the current working directory
	directory cwdir;
	Directory.current(&cwdir);

	string def_path;
	String.new(0, &def_path);
	Path.combine(&def_path, cwdir->path, __DEF_PATH, NULL);

	def_file = File.new(def_path);
	String.free(def_path);

	assert(def_file != NULL);
	_output_file_info(def_file);

	File.free(def_file);
	Allocator.flush();
}
void _file_directory()
{
	writeln("File.directory: get file directory");

	directory pDir;
	file pFile;
	File.directory(pFile = File.new(pfSource), &pDir);
	assert(pDir != NULL);
	assert(strcmp(".data", pDir->name) == 0);
	assert(strcmp("./.data", pDir->path) == 0);

	_output_directory_info(pDir);
	File.free(pFile);
	Directory.free(pDir);
}

void _dir_new()
{
	directory pDir = Directory.new(pfDir);
	assert(pDir != NULL);
	assert(strcmp(".data", pDir->name) == 0);
	assert(strcmp("./.data", pDir->path) == 0);

	_output_directory_info(pDir);
	Directory.free(pDir);
}
void _dir_exists()
{
	directory pDir = Directory.new(pfDir);
	assert(Directory.exists(pDir));

	_output_directory_info(pDir);
	Directory.free(pDir);
}
void _dir_current()
{
	directory pDir;
	Directory.current(&pDir);
	assert(pDir != NULL);

	_output_directory_info(pDir);
	Directory.free(pDir);
}

void _get_abs_path()
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
void _combine_paths()
{
	writeln("Path.combine: combine path elements with a base path");

	string expPath = "/home/david/OpenPlatform/open_framework/lib/io/.data/main.C";
	string startPath = "/home/david/OpenPlatform/open_framework/lib/io";

	string pBase;
	String.alloc(startPath, &pBase);
	writefln("pBase len: %d", String.length(pBase));

	Path.combine(&pBase, ".data", "main.C", NULL);

	assert(strcmp(expPath, pBase) == 0);
	String.free(pBase);
}
void _combine_with_empty_base()
{
	/*
		Duplicating an error condition that resulted in 2x '//' at the beginning of the path
	*/
	static const string __DEF_PATH = ".data/main.C";
	writeln("Path.combine: combine path elements from an empty base path");

	//	get the current working directory
	directory cwdir;
	Directory.current(&cwdir);

	//	this actually works ...
	string expPath = "/home/david/OpenPlatform/open_framework/lib/%s/.data/main.C";
	String.format(expPath, &expPath, cwdir->name);
	writefln("expected: %s", expPath);

	//	in the initial test, pBase is initialized with a starting path.
	string pBase;
	String.new(0, &pBase);

	Path.combine(&pBase, cwdir->path, __DEF_PATH, NULL);

	writefln("combined: %s", pBase);

	assert(strcmp(expPath, pBase) == 0);
	Directory.free(cwdir);
}
void _path_directory()
{
	writeln("Path.directory: return char* directory from path");

	string expDir = ".data";
	string pDir = Path.directory(pfDir);

	assert(strcmp(expDir, pDir) == 0);
}
void _path_file_directory()
{
	writeln("Path.directory: return char* directory from file path");

	string expDir = ".data";
	string pDir = Path.directory(pfSource);

	assert(strcmp(expDir, pDir) == 0);
}

void _get_stream_error()
{
	string expMsg = "Path not found";

	struct io_stream target = {
		.error = PATH_NOT_FOUND};

	string errMsg;
	Stream.get_error(&target, &errMsg);

	assert(strcmp(expMsg, errMsg) == 0);

	String.free(errMsg);
}
void _new_stream()
{
	//	new stream
	stream pStream = Stream.new(pfNone);

	assert(pStream != NULL);
	assert(strcmp(pfNone, pStream->source) == 0);

	_output_stream_info(pStream);
	Stream.free(pStream);
}
void _open_file_stream()
{
	//	gain access to a stream by opening a file
	file pFile = File.new(pfSource);
	stream pStream;

	assert(File.open(pFile, &pStream, READ));
	assert(strcmp(pfSource, pStream->source) == 0);
	assert(pStream->fstream != NULL);
	assert(Stream.is_open(pStream));

	_output_stream_info(pStream);
	Stream.free(pStream);
}
