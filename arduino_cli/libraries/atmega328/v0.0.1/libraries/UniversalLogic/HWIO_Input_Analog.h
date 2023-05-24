#ifndef HWIO_INPUT_ANALOG_H
#define HWIO_INPUT_ANALOG_H

#include "logic_modul.h"

class HWIO_Input_Analog : public LogicModule
{
private:
  float input();

public:
  HWIO_Input_Analog(uint16_t unique_id = 0);

  virtual uint8_t run() override;
};

#endif