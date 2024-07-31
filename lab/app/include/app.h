#ifndef APP_H
#define APP_H

#include "op/data_types.h"
#include "statum/state_machine.h"

//  App object structure
typedef struct
{
    /* data */
    const char *name;
} App;

//  AppEvent data structure
typedef struct
{
    /* data */
    const char *name;
} AppEventData;

void start(SM_StateMachine *, AppEventData **);

//  state machine event functions
EVENT_DECLARE(APP_StartUp, AppEventData);
EVENT_DECLARE(APP_Init, AppEventData);
EVENT_DECLARE(APP_Run, AppEventData);
EVENT_DECLARE(APP_ShutDown, AppEventData);
EVENT_DECLARE(APP_Exit, AppEventData);

//  get current app state
GET_DECLARE(APP_GetState, INT);
GET_DECLARE(APP_GetStateMachine, SM_StateMachine *);

#endif // APP_H