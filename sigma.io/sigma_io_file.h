/*	Created on: Aug 16, 2024
 *	Author: Badkraft
 *	
 *	sigma_io_file.h: 
 */

#ifndef _SIGMA_IO_FILE_H_
#define _SIGMA_IO_FILE_H_

typedef struct io_file
{
	enum
	{
		BINARY = 1,
		READ = 2,
		WRITE = 4,
		APPEND = 8,
		CREATE = 16
	} mode;
	char *name;
	size_t size;
	bool exists;
} file;

#endif /* _SIGMA_IO_FILE_H_ */
