#ifndef EXOSTATE_H
#define EXOSTATE_H

#include "stdbool.h"

#include "op/data_types.h"

// State machine instance data
typedef struct
{
    const CHAR *name;
    void *pInstance;

    //    BYTE newState;
    //    BYTE currentState;
    //    BOOL eventGenerated;
    //    void *pEventData;
} StateEngine;

#define EXOS(_seName_, _target_) StateEngine se_##_seName_ = {#_seName_, _target_}

#endif //  EXOSTATE_H