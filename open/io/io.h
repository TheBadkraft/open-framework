#ifndef _OPEN_IO_H
#define _OPEN_IO_H

#include "../core/core.h"

#include "io_stream.h"

enum io_type
{
    IO_NONE = -1,
    IO_UNKNOWN = 0,
    IO_FILE = 1,
    IO_DIRECTORY = 2
};

struct io_dir
{
    string path;
    string name;
};
struct io_file
{
    string path;
    string name;
};

typedef enum io_type IOType;

typedef struct io_dir *directory;
typedef struct io_file *file;

extern const struct IFile
{
    /*  members  */
    bool (*exists)(file);
    int64_t (*size)(file);
    file (*new)(string);
    file (*get)(string, int64_t *);
    void (*delete)(file);
    bool (*create)(file);
    bool (*open)(file, stream *, enum io_mode);
    void (*dispose)(file);
    void (*directory)(file, directory *);
    void (*full_path)(file, string *);
} File;

extern const struct IDirectory
{
    /*  members  */
    directory (*new)(string);
    bool (*exists)(directory);
    void (*current)(directory *);
    void (*dispose)(directory);
} Directory;

extern const struct IPath
{
    /*  members  */
    //
    bool (*absolute)(string, string *);
    //
    void (*combine)(string *, ...);
    //
    bool (*exists)(string);
    //
    char *(*directory)(string);
    //
    IOType (*type)(string);
} Path;

#endif //  _OPEN_IO_H