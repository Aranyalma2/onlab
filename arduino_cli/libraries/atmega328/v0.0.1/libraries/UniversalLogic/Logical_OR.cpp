#include "Logical_OR.h"

#define INPUT_LENGTH 2
#define OUTPUT_LENGTH 1

/* Logical OR
 * Inputs:
 * - input 1
 * - input 2
 */

Logical_OR::Logical_OR(uint16_t unique_id) : LogicModule(unique_id, INPUT_LENGTH, OUTPUT_LENGTH)
{
  // set constans end inputs
  this->setInput(0, nullptr);
  this->setInput(1, nullptr);
}

float Logical_OR::Or()
{

  if (*inputs[0] > static_cast<float>(0) || *inputs[1] > static_cast<float>(0))
  {
    return static_cast<float>(1);
  }
  else
  {
    return static_cast<float>(0);
  }
}

float Logical_OR::inverter(float in)
{
  // output is inverted
  if (in == static_cast<float>(0))
    return static_cast<float>(1);
  else
    return static_cast<float>(0);
}

uint8_t Logical_OR::run()
{
  if (this->checkValidity())
  {
    outputs[0] = this->Or();
    outputs[1] = this->inverter(outputs[0]);
    return 0;
  }
  else
  {
    return 1;
  }
}