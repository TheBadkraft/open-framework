
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "file.h"

size_t get_file_size(char *pPath)
{
    struct stat buffer;
    size_t fsize = -1;
    if (stat(pPath, &buffer) == 0)
    {
        fsize = buffer.st_size;
    }

    return fsize;
}

srcdoc *load(file *pFile)
{
    printf("loading %s\n", pFile->name);

    srcdoc *doc = malloc(sizeof(srcdoc));
    doc->pFile = pFile;

    FILE *f = fopen(pFile->name, "r");

    if (pFile->size > 0)
    {
        doc->source = malloc(pFile->size);
        size_t n = 0;
        int c;
        while ((c = fgetc(f)) != EOF)
        {
            doc->source[n++] = (char)c;
        }
        doc->source[n] = '\0';
    }

    //	set token head to NULL
    doc->pToken = NULL;

    return doc;
}
file *file_exists(char *filepath)
{
    file *pFile = malloc(sizeof(file));
    pFile->name = filepath;
    pFile->exists = access(filepath, F_OK) == 0 ? true : false;

    pFile->size = pFile->exists ? get_file_size(filepath) : -1;

    return pFile;
}
