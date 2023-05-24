#ifndef MODBUSREGISTER_LOCAL_READ_H
#define MODBUSREGISTER_LOCAL_READ_H

#include "logic_modul.h"

class ModbusRegisterLocal_Read : public LogicModule
{
private:
  float read();

public:
  ModbusRegisterLocal_Read(uint16_t unique_id = 0);

  virtual uint8_t run() override;
};

#endif