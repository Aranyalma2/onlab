#include "global_constants.h"

ModbusRTUSlave *slaveArray[1] = {new ModbusRTUSlave(9600, 1, 10, 10, 10, 10)};

const uint16_t INPUT_RESOLUTION = 4095;
const uint8_t PWM_DUTY = 255;
const float REFERNCE_VOLTAGE = 3.3;
