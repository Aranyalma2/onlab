#include "DataProcessor_DecToBit.h"
#include <Arduino.h>

#define INPUT_LENGTH 3
#define OUTPUT_LENGTH 2

// Analog-to-digital converter
// return float value 0/1
/*Inputs:
- decimal value
- switching value
*/

DataProcessor_DecToBit::DataProcessor_DecToBit(uint16_t unique_id) : LogicModule(unique_id, INPUT_LENGTH, OUTPUT_LENGTH)
{
  // maximum value which return 0 on output
  float switchingValue = 0;

  // set constans end inputs
  this->setInput(0, nullptr);
  this->setInput(1, switchingValue);
}

uint8_t DataProcessor_DecToBit::run()
{
  if (this->checkValidity())
  {
    outputs[0] = this->dtb();
    outputs[1] = this->inverter(outputs[0]);
    return 0;
  }
  else
  {
    return 1;
  }
}

float DataProcessor_DecToBit::dtb()
{
  // input higher as switching value
  if (*inputs[0] > *inputs[1])
  {
    return static_cast<float>(1);
  }
  return static_cast<float>(0);
}

float DataProcessor_DecToBit::inverter(float in)
{
  // output is inverted
  if (in == static_cast<float>(0))
    return static_cast<float>(1);
  else
    return static_cast<float>(0);
}
