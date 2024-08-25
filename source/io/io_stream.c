
#include <stdio.h>

#include "../open/io_stream.h"

typedef enum io_mode iomode;
typedef enum io_error ioerr;

char **ERR_MSGS = (char *[]){
    "No error",
    "File not found"};

bool __stream_new(file *, iomode, stream **);
//
void __stream_free(stream *);
void __stream_get_error(stream *, string **);

char *get_mode_label(iomode mode)
{
    char *pflabel = calloc(45, sizeof(char));

    if ((APPEND & mode) == APPEND)
    {
        strcpy(pflabel, "APPEND");
    }
    if ((BINARY & mode) == BINARY)
    {
        if (strcmp(pflabel, "") == 0)
        {
            strcpy(pflabel, "BINARY");
        }
        else
        {
            strcat(pflabel, " | BINARY");
        }
    }
    if ((CREATE & mode) == CREATE)
    {
        if (strcmp(pflabel, "") == 0)
        {
            strcpy(pflabel, "CREATE");
        }
        else
        {
            strcat(pflabel, " | CREATE");
        }
    }
    if ((READ & mode) == READ)
    {
        if (strcmp(pflabel, "") == 0)
        {
            strcpy(pflabel, "READ");
        }
        else
        {
            strcat(pflabel, " | READ");
        }
    }
    if ((WRITE & mode) == WRITE)
    {
        if (strcmp(pflabel, "") == 0)
        {
            strcpy(pflabel, "WRITE");
        }
        else
        {
            strcat(pflabel, " | WRITE");
        }
    }

    return pflabel;
}

char *__get_mode(iomode mode);

bool __stream_new(file *pFile, iomode mode, stream **pStream)
{
    char *pfmode = __get_mode(mode);
    printf("create new stream: %s [%s]\n", pFile->name->buffer, pfmode);

    bool retOk = false;
    iomode rw = READ | WRITE;

    (*pStream) = malloc(sizeof(stream));
    (*pStream)->source = pFile;
    (*pStream)->mode = mode;
    (*pStream)->error = NONE;
    (*pStream)->status = OK;
    (*pStream)->fstream = NULL;

    if ((CREATE & mode) == CREATE)
    {
        if (retOk = (File.exists(pFile) || File.create(pFile)))
        {
            (*pStream)->status = OK;
        }
    }
    if (strcmp("", pfmode) != 0 && ((READ & mode) == READ || (WRITE & mode) == WRITE))
    {
        if (!(retOk = File.exists(pFile)))
        {
            (*pStream)->error = FILE_NOT_FOUND;
            (*pStream)->status = ERR;
        }
    }
    if ((retOk = ((*pStream)->status != ERR)) && strcmp("", pfmode) != 0)
    {
        {
            string *path;
            File.full_path((*pStream)->source, &path);
            (*pStream)->fstream = fopen(path->buffer, pfmode);

            if ((*pStream)->fstream)
            {
                (*pStream)->status = OK;
            }
        }

        (*pStream)->length = pFile->size;
    }

    return retOk;
}
void __stream_free(stream *pStream)
{
    if (pStream)
    {
        File.free(pStream->source);
        if (pStream->fstream)
        {
            fclose(pStream->fstream);
            //  does fcloase 'free' FILE* ...???
        }

        free(pStream);
    }
}
void __stream_get_error(stream *pStream, string **pErrMsg)
{
    char *errMsg = ERR_MSGS[pStream->error];
    (*pErrMsg) = String.alloc(errMsg);
}
char *__get_err_info(ioerr err)
{
    char *errMsg = calloc(50, sizeof(char));
    strcpy(errMsg, ERR_MSGS[err]);

    return errMsg;
}
char *__get_mode(iomode mode)
{
    static char *_READ = "r";
    static char *_WRIT = "w";
    static char *_APPD = "a";
    static char *_BNRY = "b";

    char *m = calloc(3, sizeof(char));

    //  TODO: add binary mode option
    if ((READ & mode) == READ)
    {
        strcpy(m, _READ);
    }

    //  only one or the other ...
    while (true)
    {
        if ((WRITE & mode) == WRITE)
        {
            if (strcmp(m, _READ) == 0)
            {
                strcat(m, "+");
            }
            else
            {
                strcpy(m, _WRIT);
            }

            break;
        }
        if ((APPEND & mode) == APPEND)
        {
            if (strcmp(m, _READ) == 0)
            {
                strcpy(m, _APPD);
                strcat(m, "+");
            }
            else
            {
                strcpy(m, _APPD);
            }

            break;
        }

        break;
    }
    //  ????
    if ((BINARY & mode) == BINARY)
    {
        strcat("b", m);
    }

    return m;
}

const struct IO_Stream Stream = {
    .new = &__stream_new,
    .free = &__stream_free,
    .get_error = &__stream_get_error,
    .err_info = &__get_err_info,
    .mode_info = &__get_mode};