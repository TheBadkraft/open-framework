#include <stdio.h>
#include <stdlib.h>

#include "../../testing/test.h"

#include "../open/core.h"
#include "../open/io.h"
#include "../open/io_stream.h"

//	test case prototypes
void file_path_exists();
void file_size();
void create_file_obj();
void create_file();
void get_abs_path();
void combine_paths();
void new_stream_c();
void new_stream_r_fail();
void new_stream_r_pass();
void new_stream_rw_fail();
void new_stream_rw_pass();
void new_stream_rwc();
void read_stream_c_fail();
void read_stream_open();
void read_stream_c_pass();
//	utility prototypes
void __output_file_info(file *);
void __output_stream_info(stream *);

//	test values
char *pfSource = "./.data/main.C";
char *pfNone = "./.data/bad.log";
char *pfTemp = "./.data/test.log";

int main(int argc, char *argv[])
{
	write_header("OP Tests: IO");

	TEST(file_path_exists);
	TEST(file_size);
	TEST(create_file_obj);
	TEST(create_file);
	TEST(get_abs_path);
	// TEST(combine_paths);

	write_header("OP Tests: IO.Stream");
	TEST(new_stream_c);
	TEST(new_stream_r_fail);
	TEST(new_stream_r_pass);
	TEST(new_stream_rw_fail);
	TEST(new_stream_rw_pass);
	TEST(new_stream_rwc);
	TEST(read_stream_c_fail);
	TEST(read_stream_open);
	TEST(read_stream_c_pass);

	TEST_STATS();
}
void __output_file_info(file *pFile)
{
	writefln("file: %s", pFile->name->buffer);
	writefln("object:  %s", pFile != NULL ? "true" : "false");
	writefln("created: %s", File.exists(pFile) ? "true" : "false");
	writefln("size:    %ld", pFile->size);
}
void __output_stream_info(stream *pStream)
{
	writefln("file: %s", pStream->source->name->buffer);
	writefln("created: %s", File.exists(pStream->source) ? "true" : "false");
	writefln("length:  %ld", pStream->length);
	writefln("handle:  %s", pStream->fstream != NULL ? "yes" : "no");
	writefln("mode:    %s", get_mode_label(pStream->mode));
	writefln("status:  %s", pStream->status == OK ? "OK" : "ERROR");
	if (pStream->status == ERR)
	{
		string *errMsg;
		Stream.get_error(pStream, &errMsg);
		writefln("error:   %s", errMsg->buffer);

		String.free(errMsg);
	}
}
//		================================ TEST CASES ================================
void file_path_exists()
{
	writeln("path_exists: determine if path exists");
	assert(path_exists(pfSource));
	assert(path_exists(pfNone) == false);
}
void file_size()
{
	writeln("File.size: get file size");

	file *pFile = File.new(pfNone);
	File.size(pFile);

	assert(pFile->size == -1);

	free(pFile);
}
void create_file_obj()
{
	writeln("File.new: new file object");

	file *pFile = File.new(pfTemp);
	assert(pFile != NULL);
	assert(pFile->size == -1);
	assert(File.exists(pFile) == false);

	__output_file_info(pFile);
	if (File.exists(pFile))
	{
		writefln("deleting %s", pFile->name->buffer);
		File.delete(pFile);
	}

	free(pFile);
}
void create_file()
{
	writeln("File.create: create new file");

	file *pFile = File.new(pfTemp);
	assert(File.create(pFile));
	assert(pFile->size == 0);

	__output_file_info(pFile);
	if (File.exists(pFile))
	{
		writefln("deleting %s", pFile->name->buffer);
		File.delete(pFile);
	}

	File.free(pFile);
}
void get_abs_path()
{
	writeln("Path.absolute: get the absolute members of a relative path");

	char *expPath = "/home/david/OpenPlatform/open_framework/lib/io/.data/main.C";
	string *actPath; // = String.new();

	bool retOk = Path.absolute(pfSource, &actPath);
	assert(retOk);
	assert(strcmp(expPath, actPath->buffer) == 0);

	writefln("%s --> %s", pfSource, actPath->buffer);
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

	char *expPath = "/home/david/OpenPlatform/open_framework/lib/io/.data/main.C";

	string *pBase = String.alloc("/home/david/OpenPlatform/open_framework/lib/io");
	Path.combine(&pBase, ".data", "main.C", NULL);
	assert(strcmp(expPath, pBase->buffer) == 0);

	// free(pBase);
	String.free(pBase);
}
void new_stream_c()
{
	enum io_mode mode = CREATE;
	writefln("Stream.new  %s", get_mode_label(mode));

	file *pFile = File.new(pfTemp);
	stream *pStream;
	assert(Stream.new(pFile, mode, &pStream));
	assert(strcmp("\0", Stream.mode_info(pStream->mode)) == 0);

	__output_stream_info(pStream);
	if (File.exists(pFile))
	{
		writefln("deleting %s", pFile->name->buffer);
		File.delete(pFile);
	}

	Stream.free(pStream);
}
void new_stream_r_fail()
{
	enum io_mode mode = READ;
	writefln("Stream.new(ERROR: %s)  %s", Stream.err_info(FILE_NOT_FOUND), get_mode_label(mode));

	stream *pStream;
	//	TODO: Stream.get_error(pStream)
	assert(!Stream.new(File.new(pfTemp), mode, &pStream));
	assert(strcmp("r", Stream.mode_info(pStream->mode)) == 0);
	__output_stream_info(pStream);
	Stream.free(pStream);
}
void new_stream_r_pass()
{
	enum io_mode mode = READ;
	writefln("Stream.new(  %s", get_mode_label(mode));

	file *pFile = File.new(pfTemp);
	File.create(pFile);
	stream *pStream;

	assert(Stream.new(pFile, mode, &pStream));
	assert(strcmp("r", Stream.mode_info(pStream->mode)) == 0);
	__output_stream_info(pStream);
	if (File.exists(pStream->source))
	{
		writefln("deleting %s", pStream->source->name->buffer);
		File.delete(pStream->source);
	}
	Stream.free(pStream);
}
void new_stream_rw_fail()
{
	enum io_mode mode = READ | WRITE;
	writefln("Stream.new(ERROR: %s)  %s", Stream.err_info(FILE_NOT_FOUND), get_mode_label(mode));

	stream *pStream;

	assert(!Stream.new(File.new(pfTemp), mode, &pStream));
	assert(strcmp("r+", Stream.mode_info(pStream->mode)) == 0);
	__output_stream_info(pStream);
	Stream.free(pStream);
}
void new_stream_rw_pass()
{
	enum io_mode mode = READ | WRITE;
	writefln("Stream.new  %s", get_mode_label(mode));

	file *pFile = File.new(pfTemp);
	File.create(pFile);
	stream *pStream;

	assert(Stream.new(pFile, mode, &pStream));
	assert(strcmp("r+", Stream.mode_info(pStream->mode)) == 0);
	__output_stream_info(pStream);
	if (File.exists(pStream->source))
	{
		writefln("deleting %s", pStream->source->name->buffer);
		File.delete(pStream->source);
	}
	Stream.free(pStream);
}
void new_stream_rwc()
{
	enum io_mode mode = READ | WRITE | CREATE;
	writefln("Stream.new  %s", get_mode_label(mode));
	stream *pStream;

	assert(Stream.new(File.new(pfTemp), mode, &pStream));
	assert(strcmp("r+", Stream.mode_info(pStream->mode)) == 0);
	__output_stream_info(pStream);
	if (File.exists(pStream->source))
	{
		writefln("deleting %s", pStream->source->name->buffer);
		File.delete(pStream->source);
	}
	Stream.free(pStream);
}
void read_stream_c_fail()
{
	/*
		NOTE: pfSource (./.data/main.C) already exists so CREATE actually does nothing.

		When a stream is created (.new(...)) with only CREATE mode, a file stream has not yet
		been supplied by the system. We have a couple of options demonstrated in the next test cases
	*/
	enum io_mode mode = CREATE;
	writefln("Stream.read(ERROR: %s)  %s", Stream.err_info(BAD_MODE), get_mode_label(mode));
	stream *pStream;

	Stream.new(File.new(pfSource), mode, &pStream);
	char c;
	assert(!Stream.read(pStream, &c));
	__output_stream_info(pStream);
	Stream.free(pStream);
}
void read_stream_open()
{
	enum io_mode mode = CREATE;
	enum io_mode expMode = READ;
	writefln("Stream.open  %s --> %s", get_mode_label(mode), get_mode_label(expMode));
	stream *pStream;

	/*
		Option 1: when a stream object returns without error, call `.open(...)` with the desired
		modes - READ, READ | WRITE, APPEND, etc - to have the modes set and retrieve a file stream
		handle from the system.
	*/
	Stream.new(File.new(pfSource), mode, &pStream);
	assert(Stream.open(pStream, expMode));
	assert(pStream->mode == expMode);

	char c;
	assert(Stream.read(pStream, &c));
	__output_stream_info(pStream);
	Stream.free(pStream);
}
void read_stream_c_pass()
{
	enum io_mode mode = CREATE;
	enum io_mode expMode = READ;
	writefln("Stream.open  %s --> %s", get_mode_label(mode), get_mode_label(expMode));
	stream *pStream;

	/*
		Option 2: when a stream object returns without error, set the `mode` to something compatible
		with the operation - READ, READ | WRITE, APPEND, etc. This will prevent Stream from returning
		false with an *incompatible mode* complaint.
	*/
	Stream.new(File.new(pfSource), mode, &pStream);
	pStream->mode = expMode;

	char c;
	assert(Stream.read(pStream, &c));
	__output_stream_info(pStream);
	Stream.free(pStream);
}