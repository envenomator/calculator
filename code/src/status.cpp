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
    _clipToBitLength();
}

void status::_clipToBitLength()
{
    switch(_bitlength)
    {
        case 8:
            _value = _value & 0xFF;
            break;
        case 16:
            _value = _value & 0xFFFF;
            break;
        case 32:
            _value = _value & 0xFFFFFFFF;
            break;
    }
}

void status::setBase(status::Base base)
{
    _base = base;
}

void status::setBitLength(uint8_t bitlength)
{
    _bitlength = bitlength;
    _clipToBitLength();
}

uint8_t status::getBitLength()
{
    return _bitlength;
}

bool status::isNegative()
{
    switch(_bitlength)
    {
        case 8:
            return _value & 0x80;
            break;
        case 16:
            return _value & 0x8000;
            break;
        case 32:
            return _value & 0x80000000;
            break;
        default:
            break;
    }
    return false;
}

void status::setSign(bool sign)
{
    _sign = sign;
}