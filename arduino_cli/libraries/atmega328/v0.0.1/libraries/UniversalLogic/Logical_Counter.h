#ifndef LOGICAL_COUNTER_H
#define LOGICAL_COUNTER_H

#include "logic_modul.h"

class Logical_Counter : public LogicModule
{
private:
  uint8_t lastInput = 0;
  uint8_t lastReset = 0;
  float counter();

public:
  Logical_Counter(uint16_t unique_id = 0);

  virtual uint8_t run() override;
};

#endif