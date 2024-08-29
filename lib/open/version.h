#ifndef _OPEN_VERSION_H
#define _OPEN_VERSION_H

#include "core.h"

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
    string descr;
} version;

extern const struct Version_T
{
    string (*release)(relc);
    string (*version)(version *);
    string (*to_string)(version *);
    version *(*new)(byte, byte, ushort, relc, string);
} Version;

#endif //  _OPEN_VERSION_H
