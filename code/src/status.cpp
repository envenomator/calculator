#include "status.h"

status::status(uint32_t value, status::Base base, uint8_t bitlength, bool sign)
{
    _value = value;
    _base = base;
    _bitlength = bitlength;
    _sign = sign;
}

uint32_t status::getValue()
{
    return _value;
}

void status::setValue(uint32_t value)
{
    _value = value;
}

void status::setBase(status::Base base)
{
    _base = base;
}

void status::setBitLength(uint8_t bitlength)
{
    _bitlength = bitlength;
}

void status::setSign(bool sign)
{
    _sign = sign;
}