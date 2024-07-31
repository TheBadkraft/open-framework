#ifndef CENTRIFUGE_H
#define CENTRIFUGE_H

#include "op/data_types.h"
#include "statum/state_machine.h"

// Declare the private instance of CentrifugeTest state machine
SM_DECLARE(CentrifugeTestSM)

// State machine event functions
EVENT_DECLARE(CFG_Start, NoEventData)
EVENT_DECLARE(CFG_Cancel, NoEventData)
EVENT_DECLARE(CFG_Poll, NoEventData)

BOOL CFG_IsPollActive();

#endif // CENTRIFUGE_H
