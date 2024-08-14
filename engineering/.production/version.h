#ifndef _VERSION_H
#ifndef __version_build
#define __version_build "0002"
#endif
#define _VERSION_H

#include "ctypes.h"

enum RC
{
    ALPHA,
    BETA,
    RELEASE
};
#define relc enum RC

typedef struct version_t
{
    byte major;
    byte minor;
    ushort build;
    relc release;
    char *descr;
} version;

extern const struct Version_T
{
    char *(*release)(relc);
    char *(*version)(version *);
    char *(*to_string)(version *);
    version *(*new)(byte, byte, ushort, relc, char *);
} Version;

#endif //  _VERSION_H
