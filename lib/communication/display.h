#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

class Display
{
private:
    Adafruit_7segment matrix;
public:
    Display(uint8_t i2cAddress = 0x70);
    void Show(float position);
};
