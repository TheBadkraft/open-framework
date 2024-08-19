/*	Created on: Aug 16, 2024
 *	Author: Badkraft
 *	
 *	sigma_io.c: 
 */

#include <unistd.h>
#include <sys/stat.h>

#include "sigma_io.h"

// ========= File API ===============================
size_t file_size(char*);
const char *file_mode(file*);

bool file_exists(char *pPath) {
	return access(pPath, F_OK) == 0 ? true : false;
}
file* file_new(char *pPath) {

	file *pFile = malloc(sizeof(file));
	pFile->name = pPath;
	pFile->exists = file_exists(pPath);
	pFile->size = pFile->exists ? file_size(pPath) : -1;

	return pFile;
}
size_t file_size(char *pPath) {
	struct stat iostat;
	size_t fsize = -1;
	if (stat(pPath, &iostat) == 0) {
		fsize = iostat.st_size;
	}

	return fsize;
}
stream *file_open(file *pFile)
{
    if (!pFile->exists)
    {
        //  error
        return NULL;
    }

    const char *mode = file_mode(pFile);

    printf("open %s ['%s']\n", pFile->name, mode);

    stream *pStream = malloc(sizeof(stream));
    pStream->source = pFile->name;
    pStream->length = pFile->size;
    pStream->fstream = fopen(pFile->name, mode);
    pStream->is_open = pStream->fstream != NULL;
    pStream->pos = ftell(pStream->fstream);

    return pStream;
}
void file_close(stream *pStream)
{
    if (pStream->is_open)
    {
        fclose(pStream->fstream);
        pStream->is_open = false;
    }
}
char *file_get_directory(const char *pFile) {
	char *slash = strrchr(pFile, '/');
	if (!slash) {
		perror("expected path delimeter not found.");
	}

	return strndup(pFile, (++slash) - pFile);
}
const char *file_mode(file *pFile)
{
    const byte mBinary = 1;
    const byte mRead = 2;
    const byte mWrite = 4;
    const byte mAppend = 8;
    const byte mCreate = 16;

    const char *mode = String.empty;

    //  I'm certain this can be more glamorous
    if (pFile->mode == mRead)
    {
        mode = "r";
    }
    if (pFile->mode == (mRead | mWrite))
    {
        mode = "rw";
    }

    return mode;
}
const struct IO_File File = {
    .exists = &file_exists,
    .new = &file_new,
    .size = &file_size,
    .open = &file_open,
    .close = &file_close,
	.directory = &file_get_directory};

// ========= Directory API ===========================
char *dir_getcwd() {
	char *pDir;

	if((pDir=getcwd(NULL, 0)) == NULL) {
		perror("failed to get current working directory.");
	}

	return pDir;
}

const struct IO_Directory Directory = {
	.current_directory = &dir_getcwd
};


// ========= Stream API ==============================
bool stream_read(stream *pStream, char *out)
{
    int c;
    if ((c = fgetc(pStream->fstream)) != EOF)
    {
        (*out) = (char)c;
        pStream->pos = ftell(pStream->fstream);
    }

    return c != EOF;
}

const struct IO_Stream Stream = {
    .read = &stream_read};
