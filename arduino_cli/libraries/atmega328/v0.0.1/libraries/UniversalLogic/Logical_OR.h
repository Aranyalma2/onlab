#ifndef LOGICAL_OR_H
#define LOGICAL_OR_H

#include "logic_modul.h"

class Logical_OR : public LogicModule
{
private:
  float Or();
  float inverter(float in);

public:
  Logical_OR(uint16_t unique_id = 0);

  virtual uint8_t run() override;
};

#endif