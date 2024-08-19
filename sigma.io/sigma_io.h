/*	Created on: Aug 16, 2024
 *	Author: Badkraft
 *	
 *	sigma_io.h: 
 */

#ifndef _SIGMA_IO_H_
#define _SIGMA_IO_H_

#include <stdio.h>
#include <stdlib.h>

#include "sigC/sigma_types.h"
#include "sigC/sigma_string.h"

#include "sigma_io_file.h"
#include "sigma_io_directory.h"
#include "sigma_io_stream.h"

enum io_type
{
	IO_NONE = -1,
	IO_UNKNOWN = 0,
	IO_FILE = 1,
	IO_DIRECTORY = 2
};

typedef enum io_type IOType;

extern const struct IO_File
{
	bool (*exists)(char *);
	size_t (*size)(char *);
	file *(*new)(char *);
	stream *(*open)(file *);
	void (*close)(stream *);
	bool (*path_or_file)(char *, IOType *);
	char *(*directory)(const char *);
} File;

extern const struct IO_Directory
{
	char *(*current_directory)();
} Directory;

extern const struct IO_Path
{
	char *(*combine)(char *, ...);
} Path;

extern const struct IO_Stream
{
	bool (*read)(stream *, char *);
} Stream;

bool io_path_or_file(char*, IOType*);

#endif /* _SIGMA_IO_H_ */
