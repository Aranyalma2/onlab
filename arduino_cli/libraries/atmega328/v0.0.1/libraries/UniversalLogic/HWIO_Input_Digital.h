#ifndef HWIO_INPUT_DIGITAL_H
#define HWIO_INPUT_DIGITAL_H

#include "logic_modul.h"

class HWIO_Input_Digital : public LogicModule
{
private:
  uint8_t lastPinMode = 0;
  float input();

public:
  HWIO_Input_Digital(uint16_t unique_id = 0);

  virtual uint8_t run() override;
};

#endif