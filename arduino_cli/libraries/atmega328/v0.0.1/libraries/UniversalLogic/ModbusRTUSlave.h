#ifndef MODBUSRTUSLAVE_H
#define MODBUSRTUSLAVE_H

#include <Arduino.h>
#include "ModbusHandler.h"

class ModbusRTUSlave : public ModbusHandler
{
protected:
    /*
    -Coils
    -DiscreateInputs
    -HoldingRegisters
    -InputRegisters
    !Addrtessing always starts from 0 and increment type
    */
    uint8_t modbusRegisterNumber[4] = {0};

public:
    ModbusRTUSlave(uint32_t _baudrate, uint8_t _id) : ModbusHandler(_baudrate, _id) {}
    ModbusRTUSlave(uint32_t _baudrate, uint8_t _id, uint8_t _coilNum, uint8_t _discreateNum, uint8_t _holdingNum, uint8_t _inputNum) : ModbusHandler(_baudrate, _id), modbusRegisterNumber{_coilNum, _discreateNum, _holdingNum, _inputNum} {}

    /**
     * Get, the modbus register types array size
     * return array[4] : uint8_t
     */
    // uint8_t* getModbusRegNumbers(){return }

    /*
     * return 0 - success
     * return 1 - failed to start modbus
     */
    uint8_t begin();

    /*
    Polling for request handler
    return: packet packet received
    */
    uint8_t poll();

    /*
    type - Coils/DiscreateInputs/HoldingRegisters/InputRegisters
    size - new size of array
    */
    void ExtendRegisterContainer(uint8_t type, uint8_t size);

    /*
    type - Coils/DiscreateInputs/HoldingRegisters/InputRegisters
    regAddress - address of register
    value - value of the register
    */
    void WriteRegister(uint8_t type, uint8_t regAddress, int16_t value);

    /*
    type - Coils/DiscreateInputs/HoldingRegisters/InputRegisters
    regAddress - address of register
    return - value of the register
    */
    int16_t ReadRegister(uint8_t type, uint8_t regAddress);
};

#endif