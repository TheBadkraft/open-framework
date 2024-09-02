#ifndef _OPEN_VERSION_H
#define _OPEN_VERSION_H

#include "core.h"

/*
    There are some formatting and stylistic inconsistencies that need to be addressed as I am still
    developing the OpenPlatform standards.
*/

enum Open_VRC
{
    ALPHA,
    BETA,
    RELEASE
};

struct Open_Version
{
    byte major;
    byte minor;
    ushort build;
    enum Open_VRC release;
    string descr;
};

typedef enum Open_VRC RC;
typedef struct Open_Version *version;

extern const struct Version_T
{
    string (*release)(RC);
    string (*version)(version);
    string (*to_string)(version);
    version (*new)(byte, byte, ushort, RC, string);
} Version;

#endif //  _OPEN_VERSION_H
