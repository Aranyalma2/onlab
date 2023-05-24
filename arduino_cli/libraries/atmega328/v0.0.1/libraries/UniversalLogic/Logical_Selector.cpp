#include "Logical_Selector.h"

#define INPUT_LENGTH 3
#define OUTPUT_LENGTH 1

/* Logical selector block select from 2 inputs by a selector value
 * Inputs:
 * - Select input 1
 * - Select input 2
 * - Selector value
 * NOT IMPLEMENTED - Selection limit (example, if limit==0, and selector value==0 selected will be first if value==3, it will be the second one)
 */

Logical_Selector::Logical_Selector(uint16_t unique_id) : LogicModule(unique_id, INPUT_LENGTH, OUTPUT_LENGTH)
{
  // set constans end inputs
  this->setInput(0, nullptr);
  this->setInput(1, nullptr);
  this->setInput(2, nullptr);
}

float Logical_Selector::select()
{
  // selector "bit" if higher then 0 select 1, else 0
  if (*inputs[2] > static_cast<float>(0))
  {
    return *inputs[1];
  }
  else
  {
    return *inputs[0];
  }
}

uint8_t Logical_Selector::run()
{
  if (this->checkValidity())
  {
    outputs[0] = this->select();
    return 0;
  }
  else
  {
    return 1;
  }
}