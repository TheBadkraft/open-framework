#include <stdio.h>

#include "op/fb_allocator.h"
#include "statum/state_machine.h"

#include "../include/app.h"

//  define app object
static App app;

//  define state machine
SM_DEFINE(Application, &app);

int main(void)
{
    ALLOC_Init();

    AppEventData *appEvent;
    appEvent = SM_XAlloc(sizeof(App));

    //  create app event data
    start(SM_Get(Application, APP_GetStateMachine), &appEvent);

    //  call APP_Init event function to initialize application
    SM_Event(Application, APP_Init, appEvent);
}