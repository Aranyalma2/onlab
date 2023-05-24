#include "ModbusRegister_Local_Write.h"
#include <Arduino.h>
#include "ModbusRTUSlave.h"
#include "global_constants.h"

#define INPUT_LENGTH 5
#define OUTPUT_LENGTH 0

// Device local modbus register writeout logical module
// return none
/*Inputs:
- Modbus Slave Object ID
- Register type
- Register address
- Register value
- converting parameter (multiplier)
/*Outputs:
- none
*/

ModbusRegisterLocal_Write::ModbusRegisterLocal_Write(uint16_t unique_id) : LogicModule(unique_id, INPUT_LENGTH, OUTPUT_LENGTH)
{
  // this is a transformator for the write value, because modbus only can store 16bit integer values
  float multiplier = 1.0f;
  // set constans end inputs
  this->setInput(0, nullptr);
  this->setInput(1, nullptr);
  this->setInput(2, nullptr);
  this->setInput(3, nullptr);
  this->setInput(4, multiplier);
}

float ModbusRegisterLocal_Write::write()
{
  // Write to local modbus register
  int16_t value = static_cast<int16_t>(*inputs[3] * (*inputs[4]));
  slaveArray[static_cast<uint8_t>(*inputs[0])]->WriteRegister(*inputs[1], *inputs[2], value);
}

uint8_t ModbusRegisterLocal_Write::run()
{
  // Execute the logic module
  // Call write() to set the value from local memory
  // return error
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