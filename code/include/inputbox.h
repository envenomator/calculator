#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <stdint.h>
#include "displayvalue.h"

class inputBox: public displayValue
{
    //using displayValue::displayValue;

    public:
        inputBox(uint32_t value, Base base, uint8_t bitlength, Adafruit_ST7789 *screen): displayValue(value, base, bitlength, screen) {}
        void processKeyValue(unsigned char val);
};

#endif