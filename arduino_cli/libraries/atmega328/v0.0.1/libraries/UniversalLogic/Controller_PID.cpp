#include "Controller_PID.h"
#include <Arduino.h>

#define INPUT_LENGTH 6
#define OUTPUT_LENGTH 1

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
Controller_PID::Controller_PID(uint16_t unique_id) : LogicModule(unique_id, INPUT_LENGTH, OUTPUT_LENGTH) {}

uint8_t Controller_PID::run()
{
  if (this->checkValidity())
  {
    outputs[0] = this->pid_controller();
    return 0;
  }
  else
  {
    return 1;
  }
}

float Controller_PID::pid_controller()
{

  static float last_error = 0;
  static float integral = 0;

  // Calculate error
  float error = *inputs[1] - *inputs[0];

  // Calculate proportional term
  float p_term = *inputs[2] * error;

  // Calculate integral term
  integral += error * *inputs[5];
  float i_term = *inputs[3] * integral;

  // Calculate derivative term
  float d_term = *inputs[4] * (error - last_error) / *inputs[5];
  last_error = error;

  // Calculate output
  return p_term + i_term + d_term;
}