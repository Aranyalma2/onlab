#include "ModbusRTUSlave.h"
#include <ArduinoModbus.h>

uint8_t ModbusRTUSlave::begin()
{
  if (!ModbusRTUServer.begin(id, baudrate))
  {
    return 1;
  }
  for (uint8_t i = 0; i < 4; i++)
  {
    ExtendRegisterContainer(i, modbusRegisterNumber[i]);
  }
  return 0;
}

uint8_t ModbusRTUSlave::poll()
{
  // arduino
  // return ModbusRTUServer.poll();

  // esp
  ModbusRTUServer.poll();
  return 0;
}

void ModbusRTUSlave::ExtendRegisterContainer(uint8_t type, uint8_t size)
{
  switch (type)
  {
  case 0:
    modbusRegisterNumber[0] = size;
    ModbusRTUServer.configureCoils(0x00, size);
    break;
  case 1:
    modbusRegisterNumber[1] = size;
    ModbusRTUServer.configureDiscreteInputs(0x00, size);
    break;
  case 2:
    modbusRegisterNumber[2] = size;
    ModbusRTUServer.configureHoldingRegisters(0x00, size);
    break;
  case 3:
    modbusRegisterNumber[3] = size;
    ModbusRTUServer.configureInputRegisters(0x00, size);
    break;
  default:
    break;
  }
}

void ModbusRTUSlave::WriteRegister(uint8_t type, uint8_t regAddress, int16_t value)
{
  switch (type)
  {
  case 0:
    ModbusRTUServer.coilWrite(regAddress, value);
    break;
  case 1:
    ModbusRTUServer.discreteInputWrite(regAddress, value);
    break;
  case 2:
    ModbusRTUServer.holdingRegisterWrite(regAddress, value);
    break;
  case 3:
    ModbusRTUServer.inputRegisterWrite(regAddress, value);
    break;
  default:
    break;
  }
}

int16_t ModbusRTUSlave::ReadRegister(uint8_t type, uint8_t regAddress)
{
  uint16_t result = 0;
  switch (type)
  {
  case 0:
    result = ModbusRTUServer.coilRead(regAddress);
    break;
  case 1:
    result = ModbusRTUServer.discreteInputRead(regAddress);
    break;
  case 2:
    result = ModbusRTUServer.holdingRegisterRead(regAddress);
    break;
  case 3:
    result = ModbusRTUServer.inputRegisterRead(regAddress);
    break;
  default:
    break;
  }
  return result;
}