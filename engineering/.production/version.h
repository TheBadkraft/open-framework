#ifndef _VERSION_H
#ifndef __version_h_ver
#define __version_h_ver "001-a"
#endif
#define _VERSION_H

#include "sigctypes.h"

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
