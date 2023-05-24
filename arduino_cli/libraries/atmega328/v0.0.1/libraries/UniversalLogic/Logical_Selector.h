#ifndef LOGICAL_SELECTOR_H
#define LOGICAL_SELECTOR_H

#include "logic_modul.h"

class Logical_Selector : public LogicModule
{
private:
  float select();

public:
  Logical_Selector(uint16_t unique_id = 0);

  virtual uint8_t run() override;
};

#endif