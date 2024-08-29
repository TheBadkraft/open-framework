#ifndef _OPEN_IO_H
#define _OPEN_IO_H

#include "core.h"

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
    int64_t size;
};

typedef enum io_type IOType;
typedef struct io_dir directory;
typedef struct io_file file;

extern const struct IO_File
{
    /*  members  */
    bool (*exists)(file *);
    void (*size)(file *);
    file *(*new)(string);
    void (*delete)(file *);
    bool (*create)(file *);
    void (*free)(file *);
    void (*directory)(file *, directory **);
    void (*full_path)(file *, string *);
    // char *(*get_directory)(const char *);
} File;

extern const struct IO_Directory
{
    /*  members  */
    directory *(*new)(char *);
    bool (*exists)(directory *);
    void (*current)(directory **);
    void (*free)(directory *);
} Directory;

extern const struct IO_Path
{
    /*  members  */
    //
    bool (*absolute)(string, string *);
    //
    void (*combine)(string, ...);
    //
    bool (*exists)(string);
    //
    char *(*directory)(string);
    //
    IOType (*type)(string);
} Path;

#endif //  _OPEN_IO_H