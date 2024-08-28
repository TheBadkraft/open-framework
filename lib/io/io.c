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

#include "../open/io.h"

bool __file_exists(file *);
void __file_size(file *);
file *__file_new(char *);
void __get_file_path(file *, string **);
void __file_free(file *);
void __file_delete(file *);
bool __file_create(file *);
void __file_get_directory(file *, directory **);

directory *__dir_new(char *);
bool __dir_exists(directory *);
void __dir_cwd(directory **);
void __dir_free(directory *);

bool __path_absolute(char *, string **);
void __path_combine(string **, ...);
char *__path_get_directory(char *);
IOType __path_io_type(char *);
//      ===================== Utility Definitions ===========================
bool __path_exists(char *pPath)
{
    return access(pPath, F_OK) == 0 ? true : false;
}
bool path_exists(string *pPath)
{
    return __path_exists(pPath->buffer);
}
//      ======================= File Definitions ============================
bool __file_exists(file *pFile)
{
    bool retOk = false;
    if (pFile && pFile->path)
    {
        string *pPath;
        __get_file_path(pFile, &pPath);

        retOk = path_exists(pPath);
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
    size_t pathLen = strlen(pFPath) - String.length(pFile->name) - 1;
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

    bool retOk = path_exists(pPath);
    String.free(pPath);

    return retOk;
}
bool __file_truncate(file *pFile) {}
void __file_free(file *pFile)
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
void __file_get_directory(file *pFile, directory **pDir)
{
    (*pDir) = __dir_new(pFile->path->buffer);
}
void __get_file_path(file *pFile, string **pFullPath)
{
    *pFullPath = String.new();
    String.copy(*pFullPath, pFile->path->buffer);
    Path.combine(pFullPath, pFile->name->buffer, NULL);
}
//      ==================== Directory Definitions ==========================
directory *__dir_new(char *pPath)
{
    directory *pDir = malloc(sizeof(directory));
    char *slash = strrchr(pPath, '/');
    if (!slash)
    {
        perror("expected path delimeter not found.");
    }

    // char *pfDir = strndup(pPath, (++slash) - pPath);
    pDir->path = String.alloc(pPath);
    pDir->name = String.alloc(++slash);

    return pDir;
}
bool __dir_exists(directory *pDir)
{
    return path_exists(pDir->path);
}
void __dir_cwd(directory **pDir)
{
    char *pfDir;

    if ((pfDir = getcwd(NULL, 0)) == NULL)
    {
        perror("failed to get current working directory.");
    }

    (*pDir) = Directory.new(pfDir);
}
void __dir_free(directory *pDir)
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
bool __path_absolute(char *pRelPath, string **pAbsPath)
{
    bool retOk = __path_exists(pRelPath);
    *pAbsPath = NULL;

    if (retOk)
    {
        *pAbsPath = String.new();
        String.capacity(*pAbsPath, PATH_MAX);
        realpath(pRelPath, (*pAbsPath)->buffer);
    }

    return retOk;
}
void __path_combine(string **pBasePath, ...)
{
    int lastPos = 0, lastChar = 0;
    //  append the last '/'
    char *buffer = (*pBasePath)->buffer;
    if (buffer)
    {
        lastPos = strlen(buffer) - 1;
        lastChar = strrchr((*pBasePath)->buffer, '/') - buffer;
    }

    if (lastChar != lastPos)
    {
        String.append((*pBasePath), "/");
    }

    va_list args;
    va_start(args, pBasePath);
    char *arg = va_arg(args, char *);
    while (arg)
    {
        String.append((*pBasePath), arg);
        arg = va_arg(args, char *);

        if (arg)
        {
            String.append((*pBasePath), "/");
        }
    }
    va_end(args);
}
char *__path_get_directory(char *pPath)
{
    //  TODO: check for '.' or '..' and get full path
    char *pfDir = NULL;
    IOType type = __path_io_type(pPath);

    switch (type)
    {
    case IO_DIRECTORY:
        pfDir = basename(pPath);

        break;
    case IO_FILE:
        //  pfDir is a file name
        char *slash = strrchr(pPath, '/');
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
char *__path_get_file_name(char *pPath)
{
    return basename(pPath);
}
IOType __path_io_type(char *pPath)
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
const struct IO_File File = {
    /*  members  */
    .exists = &__file_exists,
    .size = &__file_size,
    .new = &__file_new,
    .delete = &__file_delete,
    .create = &__file_create,
    .free = &__file_free,
    .full_path = &__get_file_path,
    .directory = &__file_get_directory};

//		======================== Directory API ==============================
const struct IO_Directory Directory = {
    /*  members  */
    .new = &__dir_new,
    .exists = &__dir_exists,
    .current = &__dir_cwd,
    .free = &__dir_free};

//		=========================== Path API ================================
const struct IO_Path Path = {
    /*  members  */
    .absolute = &__path_absolute,
    .combine = &__path_combine,
    .directory = &__path_get_directory,
    .type = &__path_io_type};
