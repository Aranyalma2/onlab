#ifndef DATA_PROCESSOR_10KNTC_H
#define DATA_PROCESSOR_10KNTC_H

#include "logic_modul.h"

// 10k NTC Thermistor temperature degree processing
// return C/K/F
/*Inputs:
-raw input value
-resistence of sensor
-thermistor material constant
-ballast resistance
-room temp for thermistor
-output offset value
-format: output format (C,K,F)
*/
class DataProcessor_10KNTC : public LogicModule
{
private:
  const float kelvin = 273.15;
  float process();

public:
  DataProcessor_10KNTC(uint16_t unique_id = 0);

  virtual uint8_t run() override;
};
#endif
