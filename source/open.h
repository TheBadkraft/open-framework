#ifndef _OPEN__H
#define _OPEN__H

#ifndef DEBUGSO //  shared (.so) debugging
#define DEBUGSO true
#else
#define DEBUGSO false
#endif

#ifndef NDEBUG
// Production builds should set NDEBUG=1
#define NDEBUG false
#else
#define NDEBUG true
#endif

#endif //  _OPEN__H