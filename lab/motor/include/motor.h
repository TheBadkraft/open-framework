#ifndef MOTOR_H
#define MOTOR_H

#include "op/data_types.h"
#include "statum/state_machine.h"

// Motor object structure
typedef struct
{
    INT currentSpeed;
} Motor;

// Event data structure
typedef struct
{
    INT speed;
} MotorData;

// State machine event functions
EVENT_DECLARE(MTR_SetSpeed, MotorData)
EVENT_DECLARE(MTR_Halt, NoEventData)

// Public accessor
GET_DECLARE(MTR_GetSpeed, INT);

#endif // MOTOR_H
