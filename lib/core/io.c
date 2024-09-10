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

bool file_exists(file);
void file_size(file);
file file_new(string);
void get_file_path(file, string *);
void file_free(file);
void file_delete(file);
bool file_create(file);
void file_get_directory(file, directory *);

directory dir_new(string);
bool dir_exists(directory);
void dir_cwd(directory *);
void dir_free(directory);

bool path_absolute(string, string *);
void path_combine(string *, ...);
bool path_exists(string);
string path_get_directory(string);
IOType path_io_type(string);
//      ===================== Utility Definitions ===========================

//      ======================= File Definitions ============================
bool file_exists(file pFile)
{
    bool retOk = false;
    if (pFile && pFile->path)
    {
        string pPath;
        get_file_path(pFile, &pPath);

        retOk = path_exists(pPath);
        String.free(pPath);
    }

    return retOk;
}
void file_size(file pFile)
{
    struct stat iostat;
    pFile->size = -1;
    if (file_exists(pFile))
    {
        string pPath;
        get_file_path(pFile, &pPath);
        if (stat(pPath, &iostat) == 0)
        {
            pFile->size = iostat.st_size;
        }

        String.free(pPath);
    }
}
file file_new(string pFPath)
{
    size_t size = sizeof(struct io_file);
    file pFile = Allocator.alloc(sizeof(struct io_file), UNITITIALIZED);
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
    String.new(pathLen + 1, &(pFile->path));
    strncpy(pFile->path, pFPath, pathLen);
    // printf("(\"%s\")\n", pFile=>path);

    file_size(pFile);

    return pFile;
}
void file_delete(file pFile)
{
    string pPath;
    get_file_path(pFile, &pPath);

    remove(pPath);

    String.free(pPath);
}
bool file_create(file pFile)
{
    string pPath;
    get_file_path(pFile, &pPath);

    int fRet = EOF;
    FILE *pStream = fopen(pPath, "w");
    fRet = fclose(pStream);
    if (fRet != 0)
    {
        printf("[%d]ERROR closing file.\n", fRet);
    }
    file_size(pFile);

    bool retOk = path_exists(pPath);
    String.free(pPath);

    return retOk;
}
bool file_open(file pFile, stream *pStream, enum io_mode mode)
{
    bool retOk = true;
    string pFullPath;

    get_file_path(pFile, &pFullPath);
    (*pStream) = Stream.new(pFullPath);
    (*pStream)->length = pFile->size;

    if (!file_exists(pFile) && (CREATE & mode) != CREATE)
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
bool file_truncate(file pFile)
{
    //  Not Implemented
}
void file_free(file pFile)
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
        Allocator.dealloc(pFile);
    }
}
void file_get_directory(file pFile, directory *pDir)
{
    (*pDir) = dir_new(pFile->path);
}
void get_file_path(file pFile, string *pFullPath)
{
    size_t pathLen = String.length(pFile->path) + String.length("/") + String.length(pFile->name) + 1;

    String.new(pathLen, pFullPath);
    String.copy(*pFullPath, pFile->path);
    Path.combine(pFullPath, pFile->name, NULL);
}
//      ==================== Directory Definitions ==========================
directory dir_new(string pPath)
{
    directory pDir = Allocator.alloc(sizeof(struct io_dir), UNITITIALIZED);
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
bool dir_exists(directory pDir)
{
    return path_exists(pDir->path);
}
void dir_cwd(directory *pDir)
{
    string pfDir;

    if ((pfDir = getcwd(NULL, 0)) == NULL)
    {
        perror("failed to get current working directory.");
    }

    (*pDir) = Directory.new(pfDir);
}
void dir_free(directory pDir)
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
        Allocator.dealloc(pDir);
    }
}
//      ======================= Path Definitions ============================
bool path_absolute(string pRelPath, string *pAbsPath)
{
    bool retOk = path_exists(pRelPath);
    //*pAbsPath = NULL;

    if (retOk)
    {
        String.new(PATH_MAX, pAbsPath);
        realpath(pRelPath, *pAbsPath);
    }

    return retOk;
}

void path_combine(string *basePath, ...)
{
    /*
        re-engineering this method to implement String.join and more gracefully handle several factors:
        1. basePath may come in uninitialized
        2. basePath may (or may not) have a root path (not empty)
        2. basePath may (or may not) have a trailing '/'
    */
    bool hasSlash = false;
    va_list args;
    int pathLen = 0;

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
        //  first get a required length
        va_start(args, basePath);
        pathLen = str_vsnjoin("/", basePath, args) + 1;
        va_end(args);

        (*basePath) = realloc(*basePath, pathLen);

        //  we are done here, just pass everything to str_vjoin(...)
        va_list args;
        va_start(args, basePath);
        str_vjoin("/", basePath, args);
        va_end(args);
        return;
    }
    else
    {
        //  check for trailing '/'
        string slash = strrchr(*basePath, '/');
        string start = *basePath;
        size_t slashPos = slash - start;
        hasSlash = (baseLen - slashPos) == 0;
    }

    //  combine all arg members
    string dest;
    //  first get a required length
    va_start(args, basePath);
    string arg = va_arg(args, string);
    pathLen = str_vsnjoin("/", &arg, args);
    va_end(args);

    String.new(pathLen, &dest);
    va_start(args, basePath);
    str_vjoin("/", &dest, args);
    va_end(args);

    if (hasSlash)
    {
        String.resize(String.length(*basePath) + String.length(dest) + 1, basePath);
        String.append(*basePath, dest);
    }
    else
    {
        String.resize(String.length(*basePath) + String.length(dest) + 2, basePath);
        String.join("/", basePath, dest, NULL);
    }

    String.free(dest);
}

/*
void path_combine(string *pBasePath, ...)
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

bool path_exists(string pPath)
{
    return access(pPath, F_OK) == 0 ? true : false;
}
string path_get_directory(string pPath)
{
    //  TODO: check for '.' or '..' and get full path
    string pfDir = NULL;
    string pBasePath;
    IOType type = path_io_type(pPath);

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

        pfDir = path_get_directory(strndup(pPath, (++slash) - pPath));
        break;
    default:

        perror("ERR: path type unknown.");
        break;
    }

    return pfDir;
}
string path_get_file_name(string pPath)
{
    return basename(pPath);
}
IOType path_io_type(string pPath)
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
    .exists = &file_exists,
    .size = &file_size,
    .new = &file_new,
    .delete = &file_delete,
    .create = &file_create,
    .open = &file_open,
    .free = &file_free,
    .full_path = &get_file_path,
    .directory = &file_get_directory};

//		======================== Directory API ==============================
const struct Open_Directory Directory = {
    /*  members  */
    .new = &dir_new,
    .exists = &dir_exists,
    .current = &dir_cwd,
    .free = &dir_free};

//		=========================== Path API ================================
const struct Open_Path Path = {
    /*  members  */
    .absolute = &path_absolute,
    .combine = &path_combine,
    .exists = &path_exists,
    .directory = &path_get_directory,
    .type = &path_io_type};
