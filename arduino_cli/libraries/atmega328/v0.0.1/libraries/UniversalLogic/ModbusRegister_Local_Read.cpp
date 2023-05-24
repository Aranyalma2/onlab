#include "ModbusRegister_Local_Read.h"
#include <Arduino.h>
#include "ModbusRTUSlave.h"
#include "global_constants.h"

#define INPUT_LENGTH 4
#define OUTPUT_LENGTH 1

// Device local modbus register readout logical module
// return float value
/*Inputs:
- Modbus Slave Object ID
- Register type
- Register address
- converting parameter (multiplier)
/*Outputs:
- Read value
*/

ModbusRegisterLocal_Read::ModbusRegisterLocal_Read(uint16_t unique_id) : LogicModule(unique_id, INPUT_LENGTH, OUTPUT_LENGTH)
{
  // this is a transformator for the read value, because modbus only can store 16bit integer values
  float multiplier = 1.0f;
  // set constans end inputs
  this->setInput(0, nullptr);
  this->setInput(1, nullptr);
  this->setInput(2, nullptr);
  this->setInput(3, multiplier);
}

float ModbusRegisterLocal_Read::read()
{
  // Read from local modbus register and return the value
  int16_t read = slaveArray[static_cast<uint8_t>(*inputs[0])]->ReadRegister(*inputs[1], *inputs[2]);
  // conversation
  return static_cast<float>(read) * *inputs[3];
}

uint8_t ModbusRegisterLocal_Read::run()
{
  // Execute the logic module
  // Call read() to get the value from local memory
  // return error
  if (this->checkValidity())
  {
    outputs[0] = this->read();
    return 0;
  }
  else
  {
    return 1;
  }
}