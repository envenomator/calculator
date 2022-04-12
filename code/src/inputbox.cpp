#include "inputbox.h"

void inputBox::deleteDigit()
{
    int8_t temp8;
    int16_t temp16;
    int32_t temp32;

    if(_currentLength)
    {
        _displaystring[strlen(_displaystring)-1] = 0;   // delete last input character
        //update value
        switch(_base)
        {
            case Dec:
                if(_sign && status::isNegative(_value,_bitlength))
                {
                    switch(_bitlength)
                    {
                        // let the compiler handle the correct cast to sign
                        case 8:
                            temp8 = (int8_t)_value;
                            temp8 = temp8 / 10;
                            _value = temp8;
                            break;
                        case 16:
                            temp16 = (int16_t)_value;
                            temp16 = temp16 / 10;
                            _value = temp16;
                            break;
                        case 32:
                            temp32 = (int32_t)_value;
                            temp32 = temp32 / 10;
                            _value = temp32;
                            break;
                    }
                }
                else _value = _value / 10;
                break;
            case Hex:
                _value = _value >> 4;
                break;
            case Bin:
                _value = _value >> 1;
                break;
        }
        _currentLength--;
        if(_currentLength == 0)
        {
            _value = 0;
            _valueToString();
        }
        _clear();
        _display();
    }
}

void inputBox::negateDisplay()
{
    _negateValue();

    if(_show)
    {
        _clear();
        _display();
    }
}

void inputBox::_negateValue()
{
    // Negates the value (only on Dec display with sign)

    switch(_bitlength)
    {
        // negate value, let the compiler handle negation by casting to signed it
        case 8:
            _value = -(int8_t)(_value);
            break;
        case 16:
            _value = -(int16_t)(_value);
            break;
        case 32:
            _value = -(int32_t)(_value);
            break;
    }

    _valueToString();

    if(_value == 0) strcpy(_displaystring,"-"); // negate was requested on '0' (only on Dec display)
}

void inputBox::processKeyValue(unsigned char key)
{   
    // checks to see if the key is apropriate for this base
    // then checks to see if the key isn't violating the bitlength maximum
    // then enters the key into the displaystring and updates the value
    unsigned char val = key;
    uint64_t max;
    uint64_t tempval;

    bool negval = false,negtemp;

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
                if(_displaystring[0] == '-')
                {
                    // append the negative number after the '-'
                    strncat(_displaystring,(const char *)&val, 1);
                    negval = true;
                }
                else
                {
                    // Positive number or 0
                    // Just replace the on-screen '0' at the right position
                    if(_base == Dec) _displaystring[0] = key;
                    if(_base == Hex) _displaystring[2] = key;
                    if(_base == Bin) _displaystring[2] = key;
                }
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

                    if(_sign)
                    {
                        // '-' entered or actual negative _value in store
                        if(!negval) negval = status::isNegative(_value,_bitlength);

                        // add key in correct +/- fashion and cast to correct bitlength
                        switch(_bitlength)
                        {
                            case 8:
                                if(negval) tempval = (int8_t)((int8_t)(_value)*10 - (key - '0'));
                                else tempval = (int8_t)((int8_t)(_value)*10 + (key - '0'));
                                tempval &= 0xFF;
                                break;
                            case 16:
                                if(negval) tempval = (int16_t)((int16_t)(_value)*10 - (key - '0'));
                                else tempval = (int16_t)((int16_t)(_value)*10 + (key - '0'));
                                tempval &= 0xFFFF;
                                break;
                            case 32:
                                if(negval) tempval = (int32_t)((int32_t)(_value)*10 - (key - '0'));
                                else tempval = (int32_t)((int32_t)(_value)*10 + (key - '0'));
                                tempval &= 0xFFFFFFFF;
                                break;
                        }
                        negtemp = status::isNegative(tempval,_bitlength);

                        if(negval == negtemp)
                            // status unchanged: accept extra digit
                            _value = tempval;
                        else
                        {
                            _valueToString();
                            _flashWarning();
                        }
                    }
                    else // Unsigned number
                    {
                        tempval = (_value * 10) + (key - '0');
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