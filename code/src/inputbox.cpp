#include "inputbox.h"

void inputBox::processKeyValue(unsigned char key)
{
    unsigned char val = key;

    // filter apropriate key for this base
    switch(_currentBase)
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
            if(_currentBase == displayValue::Dec) _displaystring[0] = key;
            if(_currentBase == displayValue::Hex) _displaystring[2] = key;
            if(_currentBase == displayValue::Bin) _displaystring[2] = key;
        }
        else
        {
            strncat(_displaystring, (const char *)&val, 1);
        }
        _currentLength++;

        // update value
        switch(_currentBase)
        {
            case Bin:
                _currentValue = (_currentValue << 1) + (key - '0');
                break;
            case Hex:
                _currentValue = _currentValue << 4;
                if(key > '9')
                    _currentValue += 10 + key - 'A';
                else
                    _currentValue += key - '0';
                break;
            case Dec:
                _currentValue = (_currentValue * 10) + (key - '0');
                break;
            default:
                break;
        }
    }

    // display
    display();
}