
#include <stdio.h>
#include <sys/stat.h>

#include "open/io/io_stream.h"
#include "open/internal/internal_io.h"

typedef enum io_mode iomode;
typedef enum io_error ioerr;

string *ERR_MSGS = (string[]){
    "No error", "Path not found", "Incompatible mode"};

void get_friendly_mode(iomode, string *);
stream strm_new(string);
void get_err_info(ioerr, string *);
void strm_get_err_info(stream, string *);
void get_mode(iomode, string *);
bool strm_is_open(stream);
void strm_free(stream);

bool open_stream(void *p, int m)
{
    stream pStream = (stream)p;
    pStream->mode = (iomode)m;
    pStream->status = OK;
    string pMode;

    get_mode(pStream->mode, &pMode);

    pStream->fstream = fopen(pStream->source, pMode);
    if (pStream->fstream != NULL)
    {
        pStream->status |= OPEN;

        struct stat iostat;
        if (stat(pStream->source, &iostat) == 0)
        {
            pStream->length = iostat.st_size;
        }
    }
    else
    {
        pStream->status = ERR | CLOSED;
    }

    return ((OPEN & pStream->status) == OPEN);
}

void get_friendly_mode(iomode mode, string *outMode)
{
    //  45 is a swag
    (*outMode) = calloc(45, sizeof(char));

    if ((APPEND & mode) == APPEND)
    {
        strcpy((*outMode), "APPEND");
    }
    if ((BINARY & mode) == BINARY)
    {
        if (strcmp((*outMode), "") == 0)
        {
            strcpy((*outMode), "BINARY");
        }
        else
        {
            strcat((*outMode), " | BINARY");
        }
    }
    if ((CREATE & mode) == CREATE)
    {
        if (strcmp((*outMode), "") == 0)
        {
            strcpy((*outMode), "CREATE");
        }
        else
        {
            strcat((*outMode), " | CREATE");
        }
    }
    if ((READ & mode) == READ)
    {
        if (strcmp((*outMode), "") == 0)
        {
            strcpy((*outMode), "READ");
        }
        else
        {
            strcat((*outMode), " | READ");
        }
    }
    if ((WRITE & mode) == WRITE)
    {
        if (strcmp((*outMode), "") == 0)
        {
            strcpy((*outMode), "WRITE");
        }
        else
        {
            strcat((*outMode), " | WRITE");
        }
    }
}
stream strm_new(string pPath)
{
    stream pStream = Allocator.alloc(sizeof(struct io_stream), UNINITIALIZED);
    pStream->source = pPath;
    pStream->mode = NO_MODE;
    pStream->error = NONE;
    pStream->status = OK;
    pStream->fstream = NULL;
    pStream->length = 0;
    pStream->pos = 0;

    return pStream;
}
void get_err_info(ioerr err, string *pErrMsg)
{
    string errMsg = ERR_MSGS[err];
    String.alloc(errMsg, pErrMsg);
}
void strm_get_err_info(stream pStream, string *pErrMsg)
{
    get_err_info(pStream->error, pErrMsg);
}
void get_mode(iomode mode, string *pM)
{
    static char *_READ = "r";
    static char *_WRIT = "w";
    static char *_APPD = "a";
    static char *_BNRY = "b";

    *pM = calloc(3, sizeof(char));

    //  TODO: add binary mode option
    if ((READ & mode) == READ)
    {
        strcpy(*pM, _READ);
    }

    //  only one or the other ...
    while (true)
    {
        if ((WRITE & mode) == WRITE)
        {
            if (strcmp(*pM, _READ) == 0)
            {
                strcat(*pM, "+");
            }
            else
            {
                strcpy(*pM, _WRIT);
            }

            break;
        }
        if ((APPEND & mode) == APPEND)
        {
            if (strcmp(*pM, _READ) == 0)
            {
                strcpy(*pM, _APPD);
                strcat(*pM, "+");
            }
            else
            {
                strcpy(*pM, _APPD);
            }

            break;
        }

        break;
    }
    //  ????
    if ((BINARY & mode) == BINARY)
    {
        strcat("b", *pM);
    }
}
bool strm_is_open(stream pStream)
{
    bool retOk = (OPEN & pStream->status) == OPEN;
    retOk &= pStream->fstream != NULL;

    return retOk;
}
void strm_free(stream pStream)
{
    if (pStream->fstream != NULL)
    {
        if (fclose(pStream->fstream) != 0)
        {
            // error closing file
        }
        else
        {
            pStream->fstream = NULL;
        }
    }

    Allocator.dealloc(pStream);
}

const struct IStream Stream = {
    .new = &strm_new,
    .get_error = &strm_get_err_info,
    .err_info = &get_err_info,
    .mode_info = &get_mode,
    .is_open = &strm_is_open,
    .dispose = &strm_free,
};