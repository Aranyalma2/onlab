#include "HWIO_Output_Digital.h"
#include <Arduino.h>

#define INPUT_LENGTH 1
#define OUTPUT_LENGTH 1

/* HWIO_Digital_Output
value: to output
 */

HWIO_Output_Digital::HWIO_Output_Digital(uint16_t unique_id) : LogicModule(unique_id, INPUT_LENGTH, OUTPUT_LENGTH)
{
  pinMode(unique_id, OUTPUT);
}

void HWIO_Output_Digital::write()
{
  digitalWrite(unique_id, static_cast<uint16_t>(*inputs[0]));
}

uint8_t HWIO_Output_Digital::run()
{
  if (this->checkValidity())
  {
    this->write();
    return 0;
  }
  else
  {
    return 1;
  }
}