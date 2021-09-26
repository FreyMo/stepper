#include <display.h>

Display::Display(uint8_t i2cAddress) : matrix(Adafruit_7segment())
{
    matrix.begin(i2cAddress);
}

void Display::Show(float position)
{
    if (position > 999.9f || position < -99.9f)
    {
        matrix.printError();
    } else 
    {
        matrix.printFloat(position, 1);
    }

    matrix.writeDisplay();
}
