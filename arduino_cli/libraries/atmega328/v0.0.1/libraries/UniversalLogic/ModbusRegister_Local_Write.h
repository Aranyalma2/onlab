#ifndef MODBUSREGISTER_LOCAL_WRITE_H
#define MODBUSREGISTER_LOCAL_WRITE_H

#include "logic_modul.h"

class ModbusRegisterLocal_Write : public LogicModule
{
private:
  float write();

public:
  ModbusRegisterLocal_Write(uint16_t unique_id = 0);

  virtual uint8_t run() override;
};

#endif