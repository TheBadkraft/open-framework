/*	Created on: Aug 16, 2024
 *	Author: Badkraft
 *	
 *	sigma_io_stream.h: 
 */

#ifndef _SIGMA_IO_STREAM_H_
#define _SIGMA_IO_STREAM_H_



typedef struct io_stream
{
	size_t length;
	size_t pos;
	char *source;
	bool is_open;
	FILE *fstream;
} stream;

#endif /* _SIGMA_IO_STREAM_H_ */
