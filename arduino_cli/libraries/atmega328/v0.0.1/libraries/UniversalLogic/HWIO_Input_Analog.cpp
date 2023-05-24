#include "HWIO_Input_Analog.h"
#include <Arduino.h>

#define INPUT_LENGTH 0
#define OUTPUT_LENGTH 1

HWIO_Input_Analog::HWIO_Input_Analog(uint16_t unique_id) : LogicModule(unique_id, INPUT_LENGTH, OUTPUT_LENGTH) {}

float HWIO_Input_Analog::input()
{
  return static_cast<float>(analogRead(unique_id));
}

uint8_t HWIO_Input_Analog::run()
{
  if (this->checkValidity())
  {
    outputs[0] = this->input();
    return 0;
  }
  else
  {
    return 1;
  }
}