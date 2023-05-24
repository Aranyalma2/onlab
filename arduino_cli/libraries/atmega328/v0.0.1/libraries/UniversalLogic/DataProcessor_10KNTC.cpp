#include "DataProcessor_10KNTC.h"
#include <Arduino.h>
#include "global_constants.h"

#define INPUT_LENGTH 7
#define OUTPUT_LENGTH 1

/*Inputs:
-raw input value
-resistence of sensor
-thermistor material constant
-ballast resistance
-room temp for thermistor
-output offset value
-format: output format (C,K,F)
*/
DataProcessor_10KNTC::DataProcessor_10KNTC(uint16_t unique_id) : LogicModule(unique_id, INPUT_LENGTH, OUTPUT_LENGTH)
{
  /**Resistor**/
  float R = 10000;
  /**Resistor**/
  /**Thermistor datasheet**/
  float B = 3977;
  float RT0 = 10000;
  float T0 = 25;
  /**Thermistor datasheet**/
  // Output offset
  float offset = 0;
  // Output format
  float format = 0; // C,K,F

  // set constans end inputs
  this->setInput(0, nullptr);
  this->setInput(1, R);
  this->setInput(2, B);
  this->setInput(3, RT0);
  this->setInput(4, T0);
  this->setInput(5, offset);
  this->setInput(6, format);
}

uint8_t DataProcessor_10KNTC::run()
{
  if (this->checkValidity())
  {
    outputs[0] = this->process();
    return 0;
  }
  else
  {
    return 1;
  }
}

float DataProcessor_10KNTC::process()
{
  float TEMP = *inputs[4] + kelvin;
  float VRT = ((float)REFERNCE_VOLTAGE / (float)INPUT_RESOLUTION) * *inputs[0]; // Conversion to voltage
  float VR = (float)REFERNCE_VOLTAGE - VRT;
  float RT = VRT / (VR / *inputs[1]); // Resistance of RT
  float ln = log(RT / *inputs[3]);
  float out = (1 / ((ln / *inputs[2]) + (1 / TEMP))) - kelvin; // Temperature from thermistor

  // Format converter
  switch ((int)*inputs[6])
  {
  case 0:
  {
    return out + *inputs[5]; // celsius
  }
  case 1:
  {
    return out + kelvin + *inputs[5]; // kelvin
  }
  case 2:
  {
    return out * 1.8 + 32 + *inputs[5]; // fahrenheit
  }
  default:
  {
    return out + *inputs[5]; // default celsius
  }
  }
}