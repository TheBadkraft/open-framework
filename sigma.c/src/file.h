#ifndef _FILE_H
#ifndef __file_build
#define __file_build "0004"
#endif
#define _FILE_H

#include <stdio.h>

#include "ctypes.h"

enum io_type {
	IO_NONE = -1,
	IO_UNKNOWN = 0,
	IO_FILE = 1,
	IO_DIRECTORY = 2
};

typedef struct file_t {
	enum {
		BINARY = 1, READ = 2, WRITE = 4, APPEND = 8, CREATE = 16
	} mode;
	char *name;
	size_t size;
	bool exists;
} file;

typedef struct stream_t {
	size_t length;
	size_t pos;
	char *source;
	bool is_open;
	FILE *fstream;
} stream;

typedef enum io_type IOType;

extern const struct File_T {
	bool (*exists)(char*);
	size_t (*size)(char*);
	file* (*new)(char*);
	stream* (*open)(file*);
	void (*close)(stream*);
	bool (*path_or_file)(char*, IOType*);
} File;

extern const struct Stream_T {
	bool (*read)(stream*, char*);
} Stream;

#endif //  _FILE_H
