#include <display.h>

Display::Display(uint8_t i2cAddress) : matrix(Adafruit_7segment())
{
    this->matrix.begin(i2cAddress);
}

void Display::Show(float position)
{
    if (position > 999.9f || position < -99.9f)
    {
        this->matrix.printError();
    } else 
    {
        this->matrix.printFloat(position, 1);
    }

    this->matrix.writeDisplay();
    this->matrix.setBrightness(15);
}
