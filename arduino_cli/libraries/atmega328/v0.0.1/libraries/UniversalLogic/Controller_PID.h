#ifndef DATA_PROCESSOR_ADC_H
#define DATA_PROCESSOR_ADC_H

#include "logic_modul.h"

// PID controller
// return float value
/*Inputs:
-raw input value
-set point: The target value (set point) that the control block is striving to attain.
-kp: Control's Proportional setting.
-ki: Control's Integral setting.
-kd: Control's Derivative setting.
-dt: delta time, between 2 derivative term calculation
*/
class Controller_PID : public LogicModule
{
protected:
  float pid_controller();

public:
  Controller_PID(uint16_t unique_id = 0);

  virtual uint8_t run() override;
};
#endif