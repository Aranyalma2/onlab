#ifndef DATAPROCESSOR_DECTOBIT_H
#define DATAPROCESSOR_DECTOBIT_H

#include "logic_modul.h"

// Analog-to-digital value converter
// return float value 0/1
/*Inputs:
- analog value
- switching value
- work mode, direct or inverse
*/
class DataProcessor_DecToBit : public LogicModule
{
private:
  float dtb();
  float inverter(float in);

public:
  DataProcessor_DecToBit(uint16_t unique_id = 0);

  virtual uint8_t run() override;
};
#endif