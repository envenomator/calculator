#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <stdint.h>
#include "displayvalue.h"

class inputBox: public displayValue
{
    public:
        inputBox(uint32_t value, Base base, uint8_t bitlength): displayValue(value, base, bitlength) {}
        void processKeyValue(unsigned char val);
};

#endif