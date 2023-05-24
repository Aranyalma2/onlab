#ifndef HWIO_OUTPUT_PWM_H
#define HWIO_OUTPUT_PWM_H

#include "logic_modul.h"

class HWIO_Output_PWM : public LogicModule
{
private:
  void write();

public:
  HWIO_Output_PWM(uint16_t unique_id = 0);

  virtual uint8_t run() override;
};

#endif