#include <stdlib.h>

#include "../include/file.h"

file *file_exists(char *filepath)
{
    file *pFile = malloc(sizeof(file));
    pFile->name = filepath;
    pFile->exists = access(filepath, F_OK) == 0 ? true : false;

    return pFile;
}