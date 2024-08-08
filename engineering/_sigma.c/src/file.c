#include <stdlib.h>
#include <sys/stat.h>

#include "../include/file.h"

static size_t get_file_size(char *pPath)
{
    struct stat buffer;
    size_t fsize = -1;
    if (stat(pPath, &buffer) == 0)
    {
        fsize = buffer.st_size;
    }

    return fsize;
}

file *file_exists(char *filepath)
{
    file *pFile = malloc(sizeof(file));
    pFile->name = filepath;
    pFile->exists = access(filepath, F_OK) == 0 ? true : false;

    pFile->size = pFile->exists ? get_file_size(filepath) : -1;

    return pFile;
}