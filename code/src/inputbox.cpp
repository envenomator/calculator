#include "inputbox.h"

void inputBox::processKeyValue(unsigned char key)
{
    unsigned char val = key;

    // filter apropriate key for this base
    switch(_base)
    {
        case Bin:
            if(key > '1') return;
            break;
        case Dec:
            if(key > '9') return;
            break;
        case Hex:
            break;
        default:
            break;
    }

    // process key
    if(_currentLength < _currentMaxLength)
    {
        if(_currentLength == 0)
        {
            if(_base == displayValue::Dec) _displaystring[0] = key;
            if(_base == displayValue::Hex) _displaystring[2] = key;
            if(_base == displayValue::Bin) _displaystring[2] = key;
        }
        else
        {
            strncat(_displaystring, (const char *)&val, 1);
        }
        _currentLength++;

        // update value
        switch(_base)
        {
            case Bin:
                _value = (_value << 1) + (key - '0');
                break;
            case Hex:
                _value = _value << 4;
                if(key > '9')
                    _value += 10 + key - 'A';
                else
                    _value += key - '0';
                break;
            case Dec:
                _value = (_value * 10) + (key - '0');
                break;
            default:
                break;
        }
    }

    // display
    if(_show) _display();
}