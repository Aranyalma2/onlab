#include "Logical_AND.h"

#define INPUT_LENGTH 2
#define OUTPUT_LENGTH 1

/* Logical AND
 * Inputs:
 * - input 1
 * - input 2
 */

Logical_AND::Logical_AND(uint16_t unique_id) : LogicModule(unique_id, INPUT_LENGTH, OUTPUT_LENGTH)
{
  // set constans end inputs
  this->setInput(0, nullptr);
  this->setInput(1, nullptr);
}

float Logical_AND::And()
{
  if (*inputs[0] > static_cast<float>(0) && *inputs[1] > static_cast<float>(0))
  {
    return static_cast<float>(1);
  }
  else
  {
    return static_cast<float>(0);
  }
}

float Logical_AND::inverter(float in)
{
  // output is inverted
  if (in == static_cast<float>(0))
    return static_cast<float>(1);
  else
    return static_cast<float>(0);
}

uint8_t Logical_AND::run()
{
  if (this->checkValidity())
  {
    outputs[0] = this->And();
    outputs[1] = this->inverter(outputs[0]);
    return 0;
  }
  else
  {
    return 1;
  }
}