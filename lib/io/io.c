#include <stdio.h>
#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <linux/limits.h>

#include "../open/io.h"

bool __file_exists(file *);
void __file_size(file *);
file *__file_new(char *);
void __get_file_path(file *, string **);
void __file_free(file *);
void __file_delete(file *);
bool __file_create(file *);
//      ======================= File Definitions ============================
bool path_exists(char *pPath)
{
    //  TODO: should we use (string *) as the param instead of (char *) ...???
    return access(pPath, F_OK) == 0 ? true : false;
}

bool __file_exists(file *pFile)
{
    bool retOk = false;
    if (pFile && pFile->path)
    {
        string *pPath;
        __get_file_path(pFile, &pPath);

        retOk = path_exists(pPath->buffer);
        String.free(pPath);
    }

    return retOk;
}
void __file_size(file *pFile)
{
    struct stat iostat;
    pFile->size = -1;
    if (__file_exists(pFile))
    {
        string *pPath;
        __get_file_path(pFile, &pPath);
        if (stat(pPath->buffer, &iostat) == 0)
        {
            pFile->size = iostat.st_size;
        }

        String.free(pPath);
    }
}
file *__file_new(char *pFPath)
{
    file *pFile = malloc(sizeof(file));
    if (!pFile)
    {
        //  handle error
        return NULL;
    }
    // printf("File.new[%s] ", pFPath);

    //  function needs to go to Path but we need to know if it's a directory or file
    pFile->name = String.alloc(basename(pFPath));
    // printf("%s ", pFile->name->buffer);

    //  this is a get_path function for Path
    size_t pathLen = strlen(pFPath) - String.length(pFile->name);
    char *path = calloc(pathLen, sizeof(char));
    strncpy(path, pFPath, pathLen);

    pFile->path = String.alloc(path);
    // printf("(\"%s\")\n", pFile->path->buffer);

    __file_size(pFile);

    return pFile;
}
void __file_delete(file *pFile)
{
    string *pPath;
    __get_file_path(pFile, &pPath);

    remove(pPath->buffer);

    String.free(pPath);
}
bool __file_create(file *pFile)
{
    string *pPath;
    __get_file_path(pFile, &pPath);

    FILE *pStream = fopen(pPath->buffer, "w");
    fclose(pStream);
    __file_size(pFile);

    bool retOk = path_exists(pPath->buffer);

    String.free(pPath);

    return retOk;
}
bool __file_truncate(file *pFile) {}
void __get_file_path(file *pFile, string **pFullPath)
{
    *pFullPath = String.new();
    String.copy(*pFullPath, pFile->path->buffer);
    Path.combine(pFullPath, pFile->name->buffer, NULL);
}
void __file_free(file *pFile)
{
    if (pFile)
    {
        if (pFile->name != NULL)
        {
            String.free(pFile->name);
        }
        if (pFile->path)
        {
            String.free(pFile->path);
        }
        free(pFile);
    }
}
//      ======================= Path Definitions ============================
bool __path_get_abs(char *pRelPath, string **pAbsPath)
{
    bool retOk = path_exists(pRelPath);
    *pAbsPath = NULL;

    if (retOk)
    {
        *pAbsPath = String.new();
        String.capacity(*pAbsPath, PATH_MAX);
        realpath(pRelPath, (*pAbsPath)->buffer);
        //*pAbsPath = pPath;
    }

    return retOk;
}
void __path_combine(string **pBasePath, ...)
{
    //  append the last '/'
    char *buffer = (*pBasePath)->buffer;
    int lastPos = strlen(buffer) - 1;
    int lastChar = strrchr((*pBasePath)->buffer, '/') - buffer;

    if (lastChar != lastPos)
    {
        String.append(*pBasePath, "/");
    }

    va_list args;
    va_start(args, pBasePath);
    char *arg = va_arg(args, char *);
    while (arg)
    {
        String.append(*pBasePath, arg);
        arg = va_arg(args, char *);

        if (arg)
        {
            String.append(*pBasePath, "/");
        }
    }
    va_end(args);
}
//		=========================== File API ================================
const struct IO_File File = {
    /*  members  */
    .exists = &__file_exists,
    .size = &__file_size,
    .new = &__file_new,
    .delete = &__file_delete,
    .create = &__file_create,
    .free = &__file_free,
    .full_path = &__get_file_path,
    // .get_directory = &file_get_directory,
};

const struct IO_Directory Directory = {
    /*  members  */
    // .current_directory = &dir_getcwd
};

//		=========================== Path API ================================
const struct IO_Path Path = {
    /*  members  */
    .absolute = &__path_get_abs,
    .combine = &__path_combine};
