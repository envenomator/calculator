#include "inputbox.h"

void inputBox::deleteDigit()
{
    if(_currentLength)
    {
        _displaystring[strlen(_displaystring)-1] = 0;   // delete last input character
        //update value
        switch(_base)
        {
            case Dec:
                _value = _value / 10;
                break;
            case Hex:
                _value = _value >> 4;
                break;
            case Bin:
                _value = _value >> 1;
                break;
        }
        _currentLength--;
        if(_currentLength == 0) strcat(_displaystring, "0");
        _clear();
        _display();
    }
}

void inputBox::processKeyValue(unsigned char key)
{
    unsigned char val = key;
    uint64_t max;
    uint64_t tempval;

    // filter apropriate key for this base
    switch(_base)
    {
        case Bin:
            if(key > '1') 
            {
                _flashWarning();
                _display();
                return;
            }
            break;
        case Dec:
            if(key > '9')
            {
                _flashWarning();
                _display();
                return;
            } 
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
            if(key != '0') // don't process extra zeroes when 0 on screen
            {
                // Just replace the on-screen '0' at the right position
                if(_base == Dec) _displaystring[0] = key;
                if(_base == Hex) _displaystring[2] = key;
                if(_base == Bin) _displaystring[2] = key;
                _currentLength++;
            }
        }
        else
        {
            strncat(_displaystring, (const char *)&val, 1);
            _currentLength++;
        }

        if(_currentLength) // zero could be pressed with '0' on screen, so no processing then
        {
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
                    tempval = (_value * 10) + (key - '0');

                    if(_sign)
                    {
                        if(status::isNegative(_value, _bitlength) == status::isNegative(tempval, _bitlength))
                            // status unchanged: accept extra digit
                            _value = tempval;
                        else
                        {
                            _valueToString();
                            _flashWarning();
                        }
                    }
                    else
                    {
                        // Unsigned integer in decimal
                        // check if we overshoot maximum values
                        switch(_bitlength)
                        {
                            case 8:
                                max = 0xFF;
                                break;
                            case 16:
                                max = 0xFFFF;
                                break;
                            case 32:
                                max = 0xFFFFFFFF;
                                break;
                        }
                        if(tempval > max)
                        {
                            _valueToString();
                            _flashWarning();
                        }
                        else _value = tempval; // accept change
                    }
                    break;
                default:
                    break;
            }
        }
    }
    else _flashWarning();

    // display
    if(_show) _display();
}

void inputBox::_flashWarning()
{
    uint16_t tempcolor;

    // Briefly flash RED text, before returning to normal color
    tempcolor = _fgcolor;
    _fgcolor = ST77XX_RED;
    _clear();
    _display();
    _fgcolor = tempcolor;
    _clear();
}