#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

// GENERATE THIS FILE DYNAMICLY
#include "ModbusRTUSlave.h"

extern ModbusRTUSlave *slaveArray[];

extern const uint16_t INPUT_RESOLUTION;
extern const uint8_t PWM_DUTY;
extern const float REFERNCE_VOLTAGE;

#endif