#ifndef LOGICAL_AND_H
#define LOGICAL_AND_H

#include "logic_modul.h"

class Logical_AND : public LogicModule
{
private:
  float And();
  float inverter(float in);

public:
  Logical_AND(uint16_t unique_id = 0);

  virtual uint8_t run() override;
};

#endif