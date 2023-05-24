#ifndef HWIO_OUTPUT_DIGITAL_H
#define HWIO_OUTPUT_DIGITAL_H

#include "logic_modul.h"

class HWIO_Output_Digital : public LogicModule
{
private:
  void write();

public:
  HWIO_Output_Digital(uint16_t unique_id = 0);

  virtual uint8_t run() override;
};

#endif