//  reader & tokenizer

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "../include/loader.h"

static bool is_initialized = false;
static srcdoc *doc;

void loader_init()
{
    //  TODO: ...??? any pre-initialization ???
    is_initialized = true;
}
srcdoc *loader_load(file *pFile)
{
    printf("loading %s\n", pFile->name);

    doc = malloc(sizeof(srcdoc));
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

    return doc;
}
void loader_destroy()
{
    free(doc);
}

const struct Loader_T Loader = {
    .init = &loader_init,
    .load = &loader_load,
    .destroy = &loader_destroy};