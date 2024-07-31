#include <stdio.h>
#include <string.h>

#include "../include/app.h"

enum States
{
    ST_STARTUP,  //  start up application
    ST_INIT,     //  initialize resources
    ST_RUN,      //  start application
    ST_SHUTDOWN, //  application shut down
    ST_EXIT      //  exit application
};

char const *const State_Name[] =
    {
        "StartUp",
        "Init",
        "Run",
        "ShutDown",
        "Exit"};

//  state functions
STATE_DECLARE(StartUp, AppEventData)
STATE_DECLARE(Init, AppEventData)
STATE_DECLARE(Run, NoEventData)
STATE_DECLARE(ShutDown, NoEventData)
STATE_DECLARE(Exit, NoEventData)

//  map state function order
BEGIN_STATE_MAP(App)
/**/ STATE_MAP_ENTRY(ST_StartUp)
/**/ STATE_MAP_ENTRY(ST_Init)
/**/ STATE_MAP_ENTRY(ST_Run)
/**/ STATE_MAP_ENTRY(ST_ShutDown)
/**/ STATE_MAP_ENTRY(ST_Exit)
END_STATE_MAP(App)

//  external event definitions ***************
EVENT_DEFINE(APP_Init, AppEventData){
    /*  mapping occurs in the order states are defined  */
    BEGIN_TRANSITION_MAP                //  _ current state _
        TRANSITION_MAP_ENTRY(ST_INIT)   //      ST_StartUp
    TRANSITION_MAP_ENTRY(EVENT_IGNORED) //      ST_Init
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //      ST_Run
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //      ST_ShutDown
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //      ST_Exit
    END_TRANSITION_MAP(App, pEventData)}

EVENT_DEFINE(APP_Run, AppEventData){
    BEGIN_TRANSITION_MAP                    //  _ current state _
        TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //      ST_StartUp
    TRANSITION_MAP_ENTRY(ST_RUN)            //      ST_Init
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     //      ST_Run
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     //      ST_ShutDown
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     //      ST_Exit
    END_TRANSITION_MAP(App, pEventData)}

EVENT_DEFINE(APP_ShutDown, AppEventData){
    BEGIN_TRANSITION_MAP                    //  _ current state _
        TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //      ST_StartUp
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     //      ST_Init
    TRANSITION_MAP_ENTRY(ST_SHUTDOWN)       //      ST_Run
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     //      ST_ShutDown
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     //      ST_Exit
    END_TRANSITION_MAP(App, pEventData)}

EVENT_DEFINE(APP_Exit, AppEventData){
    BEGIN_TRANSITION_MAP                    //  _ current state _
        TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //      ST_StartUp
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     //      ST_Init
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     //      ST_Run
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     //      ST_ShutDown
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     //      ST_Exit
    END_TRANSITION_MAP(App, pEventData)}
//  ******************************************

//  state command definitions ****************
//  start up state
STATE_DEFINE(StartUp, AppEventData)
{
    printf("%s[%s]\n", self->name, State_Name[self->currentState]);
}
//  init state
STATE_DEFINE(Init, AppEventData)
{
    // Get pointer to the app instance data
    App *pInstance = SM_GetInstance(App);
    pInstance->name = pEventData->name;

    printf("%s[%s]\n", pInstance->name, State_Name[self->currentState]);
}
//  run state
STATE_DEFINE(Run, NoEventData)
{
    printf("%s ST_Run\n", self->name);
}
//  shut down state
STATE_DEFINE(ShutDown, NoEventData)
{
    printf("%s ST_ShutDown\n", self->name);
}
//  exit state
STATE_DEFINE(Exit, NoEventData)
{
    printf("%s ST_Exit\n", self->name);
}
//  ******************************************

//  getter definitions  **********************
GET_DEFINE(APP_GetState, INT)
{
    return self->currentState;
}
GET_DEFINE(APP_GetStateMachine, SM_StateMachine *)
{
    return self;
}
//  ******************************************
const char *get_current_state(INT state)
{
    return State_Name[state];
}

void start(SM_StateMachine *sm, AppEventData **appEvent)
{
    INT state = sm->currentState;
    // strcpy(appEvent->name, sm->name);
    (*appEvent)->name = sm->name;

    printf("%s[%s]\n", (*appEvent)->name, State_Name[state]);
}