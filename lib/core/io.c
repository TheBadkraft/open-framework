#include <stdio.h>
#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <linux/limits.h>

#include "open/io.h"
#include "open/internal/internal_io.h"
#include "open/internal/internal_string.h"

bool __file_exists(file);
void __file_size(file);
file __file_new(string);
void __get_file_path(file, string *);
void __file_free(file);
void __file_delete(file);
bool __file_create(file);
void __file_get_directory(file, directory *);

directory __dir_new(string);
bool __dir_exists(directory);
void __dir_cwd(directory *);
void __dir_free(directory);

bool __path_absolute(string, string *);
void __path_combine(string *, ...);
bool __path_exists(string);
string __path_get_directory(string);
IOType __path_io_type(string);
//      ===================== Utility Definitions ===========================

//      ======================= File Definitions ============================
bool __file_exists(file pFile)
{
    bool retOk = false;
    if (pFile && pFile->path)
    {
        string pPath;
        __get_file_path(pFile, &pPath);

        retOk = __path_exists(pPath);
        String.free(pPath);
    }

    return retOk;
}
void __file_size(file pFile)
{
    struct stat iostat;
    pFile->size = -1;
    if (__file_exists(pFile))
    {
        string pPath;
        __get_file_path(pFile, &pPath);
        if (stat(pPath, &iostat) == 0)
        {
            pFile->size = iostat.st_size;
        }

        String.free(pPath);
    }
}
file __file_new(string pFPath)
{
    file pFile = malloc(sizeof(struct io_file));
    if (!pFile)
    {
        //  handle error
        return NULL;
    }
    // printf("File.new[%s] ", pFPath);

    //  function needs to go to Path but we need to know if it's a directory or file
    string baseName = basename(pFPath);
    String.alloc(baseName, &(pFile->name));
    // printf("%s ", pFile->name->buffer);

    size_t pathLen = strlen(pFPath) - String.length(pFile->name) - 1;
    String.new(pathLen, &(pFile->path));
    strncpy(pFile->path, pFPath, pathLen);
    // printf("(\"%s\")\n", pFile=>path);

    __file_size(pFile);

    return pFile;
}
void __file_delete(file pFile)
{
    string pPath;
    __get_file_path(pFile, &pPath);

    remove(pPath);

    String.free(pPath);
}
bool __file_create(file pFile)
{
    string pPath;
    __get_file_path(pFile, &pPath);

    int fRet = EOF;
    FILE *pStream = fopen(pPath, "w");
    fRet = fclose(pStream);
    if (fRet != 0)
    {
        printf("[%d]ERROR closing file.\n", fRet);
    }
    __file_size(pFile);

    bool retOk = __path_exists(pPath);
    String.free(pPath);

    return retOk;
}
bool __file_open(file pFile, stream *pStream, enum io_mode mode)
{
    bool retOk = true;
    string pFullPath;

    __get_file_path(pFile, &pFullPath);
    (*pStream) = Stream.new(pFullPath);
    (*pStream)->length = pFile->size;

    if (!__file_exists(pFile) && (CREATE & mode) != CREATE)
    {
        (*pStream)->error = PATH_NOT_FOUND;
        retOk = false;
    }
    if (retOk)
    {
        __open_stream(*pStream, mode);
    }

    return retOk;
}
bool __file_truncate(file pFile)
{
    //  Not Implemented
}
void __file_free(file pFile)
{
    if (pFile)
    {
        if (pFile->name)
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
void __file_get_directory(file pFile, directory *pDir)
{
    (*pDir) = __dir_new(pFile->path);
}
void __get_file_path(file pFile, string *pFullPath)
{
    String.new(0, pFullPath);
    String.copy(*pFullPath, pFile->path);
    Path.combine(pFullPath, pFile->name, NULL);
}
//      ==================== Directory Definitions ==========================
directory __dir_new(string pPath)
{
    directory pDir = malloc(sizeof(struct io_dir));
    string slash = strrchr(pPath, '/');
    if (!slash)
    {
        perror("expected path delimeter not found.");
    }

    // string pfDir = strndup(pPath, (++slash) - pPath);
    String.alloc(pPath, &(pDir->path));
    String.alloc(++slash, &(pDir->name));

    return pDir;
}
bool __dir_exists(directory pDir)
{
    return __path_exists(pDir->path);
}
void __dir_cwd(directory *pDir)
{
    string pfDir;

    if ((pfDir = getcwd(NULL, 0)) == NULL)
    {
        perror("failed to get current working directory.");
    }

    (*pDir) = Directory.new(pfDir);
}
void __dir_free(directory pDir)
{
    if (pDir)
    {
        if (pDir->name)
        {
            String.free(pDir->name);
        }
        if (pDir->path)
        {
            String.free(pDir->path);
        }
        free(pDir);
    }
}
//      ======================= Path Definitions ============================
bool __path_absolute(string pRelPath, string *pAbsPath)
{
    bool retOk = __path_exists(pRelPath);
    //*pAbsPath = NULL;

    if (retOk)
    {
        String.new(PATH_MAX, pAbsPath);
        realpath(pRelPath, *pAbsPath);
    }

    return retOk;
}

void __path_combine(string *basePath, ...)
{
    /*
        re-engineering this method to implement String.join and more gracefully handle several factors:
        1. basePath may come in uninitialized
        2. basePath may (or may not) have a root path (not empty)
        2. basePath may (or may not) have a trailing '/'
    */
    bool hasSlash = false;

    //  freshly initialized
    if (basePath == NULL)
    {
        //  also means there can be no trailing '/'
        String.new(0, basePath);
    }
    size_t baseLen = String.length(*basePath);
    //  if empty then there is no trailing '/'
    if (baseLen == 0)
    {
        //  we are done here, just pass everything to __str_vjoin(...)
        va_list args;
        va_start(args, basePath);
        __str_vjoin("/", basePath, args);
        va_end(args);
        return;
    }
    else
    {
        //  check for trailing '/'
        string slash = strrchr(*basePath, '/');
        string start = *basePath;
        size_t slashPos = slash - start;
        hasSlash = (baseLen - slashPos) > 0;
    }

    //  combine all arg members
    string dest;
    String.new(0, &dest);
    va_list args;

    //  first get a required length
    va_start(args, basePath);
    int pathLen = __str_vsnjoin("/", &dest, args);
    va_end(args);

    dest = realloc(dest, pathLen);
    va_start(args, basePath);
    __str_vjoin("/", &dest, args);
    va_end(args);

    if (hasSlash)
    {
        String.append(*basePath, dest);
    }
    else
    {
        String.join("/", basePath, dest);
    }
}

/*
void __path_combine(string *pBasePath, ...)
{
    int hasSlash = 0;
    size_t pos = (strrchr(*pBasePath, '/') - *pBasePath);
    hasSlash = String.length(*pBasePath) - pos;
    size_t len = String.length(*pBasePath);
    va_list args;

    //  first determine how much to resize (realloc) base path
    if (hasSlash != 0 && len > 0)
    {
        len += String.length("/");
    }
    va_start(args, pBasePath);
    string arg = va_arg(args, string);
    while (arg)
    {
        len += String.length(arg);
        arg = va_arg(args, string);
        if (arg)
        {
            // String.append(pBasePath, "/");
            len += String.length("/");
        }
    }
    va_end(args);

    //  now reallocate base path
    (*pBasePath) = realloc(*pBasePath, len + 1);
    if (hasSlash != 0 && !String.is_empty(*pBasePath))
    {
        String.append(*pBasePath, "/");
    }
    va_start(args, pBasePath);
    arg = va_arg(args, string);
    while (arg)
    {
        String.append(*pBasePath, arg);
        arg = va_arg(args, string);
        if (arg)
        {
            String.append(*pBasePath, "/");
        }
    }
    va_end(args);
}
 */

bool __path_exists(string pPath)
{
    return access(pPath, F_OK) == 0 ? true : false;
}
string __path_get_directory(string pPath)
{
    //  TODO: check for '.' or '..' and get full path
    string pfDir = NULL;
    string pBasePath;
    IOType type = __path_io_type(pPath);

    switch (type)
    {
    case IO_DIRECTORY:
        pfDir = basename(pPath);

        break;
    case IO_FILE:
        //  pfDir is a file name
        string slash = strrchr(pPath, '/');
        if (!slash)
        {
            perror("expected path delimeter not found.");
        }

        pfDir = __path_get_directory(strndup(pPath, (++slash) - pPath));
        break;
    default:

        perror("ERR: path type unknown.");
        break;
    }

    return pfDir;
}
string __path_get_file_name(string pPath)
{
    return basename(pPath);
}
IOType __path_io_type(string pPath)
{
    DIR *dir = opendir(pPath);

    if (dir != NULL)
    {
        closedir(dir);
        return IO_DIRECTORY;
    }

    if (errno == ENOTDIR)
    {
        return IO_FILE;
    }

    return IO_UNKNOWN;
}
//		=========================== File API ================================
const struct Open_File File = {
    /*  members  */
    .exists = &__file_exists,
    .size = &__file_size,
    .new = &__file_new,
    .delete = &__file_delete,
    .create = &__file_create,
    .open = &__file_open,
    .free = &__file_free,
    .full_path = &__get_file_path,
    .directory = &__file_get_directory};

//		======================== Directory API ==============================
const struct Open_Directory Directory = {
    /*  members  */
    .new = &__dir_new,
    .exists = &__dir_exists,
    .current = &__dir_cwd,
    .free = &__dir_free};

//		=========================== Path API ================================
const struct Open_Path Path = {
    /*  members  */
    .absolute = &__path_absolute,
    .combine = &__path_combine,
    .exists = &__path_exists,
    .directory = &__path_get_directory,
    .type = &__path_io_type};
