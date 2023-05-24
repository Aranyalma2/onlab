#ifndef MODBUSHANDLER_H
#define MODBUSHANDLER_H

class ModbusHandler
{
protected:
  uint32_t baudrate;
  uint8_t id;
  ModbusHandler(uint32_t _baudrate, uint8_t _id) : baudrate(_baudrate), id(_id) {}
};

#endif