#ifndef _OPEN_IO_H
#define _OPEN_IO_H

#include "core.h"
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

extern const struct Open_File
{
    /*  members  */
    bool (*exists)(file);
    int64_t (*size)(file);
    file (*new)(string);
    file (*get)(string, int64_t *);
    void (*delete)(file);
    bool (*create)(file);
    bool (*open)(file, stream *, enum io_mode);
    void (*free)(file);
    void (*directory)(file, directory *);
    void (*full_path)(file, string *);
} File;

extern const struct Open_Directory
{
    /*  members  */
    directory (*new)(string);
    bool (*exists)(directory);
    void (*current)(directory *);
    void (*free)(directory);
} Directory;

extern const struct Open_Path
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