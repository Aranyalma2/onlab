#include "Logical_AND.h"
#include "HWIO_Input_Analog.h"
#include "HWIO_Output_PWM.h"
LogicModule *globalBlockContainer[2];
uint16_t globalBlockContainerSize=2;
LogicModule *globalInputContainer[1];
uint16_t globalInputContainerSize=1;
LogicModule *globalOutputContainer[1];
uint16_t globalOutputContainerSize=1;
void setup(){
globalInputContainer[0]=new HWIO_Input_Analog(1);
globalOutputContainer[0]=new HWIO_Output_PWM(2);
globalBlockContainer[0]=new Logical_AND(0);
globalBlockContainer[1]=new Logical_AND(1);
globalBlockContainer[0]->setInput(0, *(globalBlockContainer[0]->getOutput(0)));
globalBlockContainer[0]->setInput(1, 211);
globalBlockContainer[0]->setInput(2, *(globalInputContainer[0]->getOutput(0)));
globalBlockContainer[1]->setInput(0, 0);
globalInputContainer[0]->setInput(0, *(globalBlockContainer[1]->getOutput(0)));
globalOutputContainer[0]->setInput(0, *(globalInputContainer[0]->getOutput(0)));
};
void loop(){
for(uint16_t i = 0; i < globalInputContainerSize; i++){globalInputContainer[i]->run();}
for(uint16_t i = 0; i < globalBlockContainerSize; i++){globalBlockContainer[i]->run();}
for(uint16_t i = 0; i < globalOutputContainerSize; i++){globalOutputContainer[i]->run();}};
