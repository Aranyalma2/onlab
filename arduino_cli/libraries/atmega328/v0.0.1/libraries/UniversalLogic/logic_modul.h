#ifndef LOGIC_MODULE_H
#define LOGIC_MODULE_H

#include <Arduino.h>

/********************************
Logical modules primitive class
contains:   -IO list - float lists
            -Run function
            -validity IO check
            -details function for IO handler
********************************/

class LogicModule
{
protected:
    uint16_t unique_id;

    float **inputs;
    float *outputs;
    float *myConstants;
    uint8_t inputSize;
    uint8_t outputSize;

public:
    LogicModule(const uint16_t _unique_id, uint8_t inputLength, uint8_t outputLength)
    {
        unique_id = _unique_id;

        inputs = new float *[inputLength] {};
        outputs = new float[outputLength]();
        myConstants = new float[inputLength]();
        inputSize = inputLength;
        outputSize = outputLength;
    };
    virtual ~LogicModule()
    {
        delete[] inputs;
        delete[] outputs;
        delete[] myConstants;
    }

    // get number of inputs
    uint8_t getInputsNumber() const
    {
        return inputSize;
    };

    // get number of outputs
    uint8_t getOutputsNumber() const
    {
        return outputSize;
    };

    // getInputArray
    float **getInputs()
    {
        return inputs;
    }

    // get any output pointer
    float *getOutput(uint8_t id = 0)
    {
        if (id >= getOutputsNumber())
        {
            id = 0;
        }
        return &outputs[id];
    }

    // If user set a const to the module input, store it as local const container and use that pointer in *inputs[id]
    void setInput(uint8_t id, float value)
    {
        if (getInputsNumber() >= id)
        {
            myConstants[id] = value;
            inputs[id] = &myConstants[id];
        }
    }

    // If user set a const to the module input, store it as local const container and use that pointer in *inputs[id]
    //int cast float for pure function call
    void setInput(uint8_t id, int value)
    {
        if (getInputsNumber() >= id)
        {
            myConstants[id] = static_cast<float>(value);
            inputs[id] = &myConstants[id];
        }
    }

    // If user set another module/input/etc as input, set that pointer to the *inputs[id]
    void setInput(uint8_t id, float *value)
    {
        if (getInputsNumber() >= id)
            inputs[id] = value;
    }

    // check block validity, all input is valid
    bool checkValidity()
    {
        for (uint8_t i = 0; i < inputSize; i++)
        {
            if (inputs[i] == nullptr)
            {
                return false;
            }
        }
        return true;
    }

    // DEBUG MSG for dev purpose. Log inputs, outputs, constants
    void debug()
    {
        Serial.println(F("LogicModul"));
    }

    // Execute logic calc function
    virtual uint8_t run() = 0;

    // virtual String getDetails() const = 0;
};

#endif