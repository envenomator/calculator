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

    _clipToBitLength();
    _valueToString();

    if(_value == 0) strcpy(_displaystring,"-"); // negate was requested on '0' (only on Dec display)
}

void inputBox::processKeyValue(unsigned char key)
{   
    // checks to see if the key is apropriate for this base
    // then checks to see if the key isn't violating the bitlength maximum
    // then enters the key into the displaystring and updates the value
    unsigned char val = key;
    uint8_t n;
    uint32_t t1,t2;
    uint32_t max;
    uint32_t min;
    uint64_t tempval,tempval2;
    bool overflow;
    bool acceptinput = false;

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
            // first digit to enter
            if(key != '0') // don't process extra zeroes when 0 on screen
            {
                if(_displaystring[0] == '-') // - on screen?
                {
                    // append the digit after the '-'
                    strncat(_displaystring,(const char *)&val, 1);
                }
                else // 0 on screen
                {
                    // Just replace the on-screen '0' at the right position
                    if(_base == Dec) _displaystring[0] = key;
                    if(_base == Hex) _displaystring[2] = key;
                    if(_base == Bin) _displaystring[2] = key;
                }
                _currentLength++;
            }
        }
        else // 2nd digit to enter, just append it
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
                    acceptinput = true;
                    break;
                case Hex:
                    _value = _value << 4;
                    if(key > '9')
                        _value += 10 + key - 'A';
                    else
                        _value += key - '0';
                    
                    acceptinput = true;
                    break;
                case Dec:
                    if(_sign)
                    {
                        // signed number
                        // configure min/max for positive/negative number in signed
                        switch(_bitlength)
                        {
                            case 8:
                                max = 0x7F;
                                min = 0x80;
                                break;
                            case 16:
                                max = 0x7FFF;
                                min = 0x8000;
                                break;
                            case 32:
                                max = 0x7FFFFFFF;
                                min = 0x80000000;
                                break;
                        }
                        if(_displaystring[0] == '-') 
                        {
                            // negative number in input
                            // start out with (key - '0'), so _value becomes negative always first
                            if(_currentLength == 1)
                            {
                                tempval = -(key - '0');
                                tempval = status::clipToBitLength(tempval, _bitlength);
                                acceptinput = true;
                            }  
                            // for subsequent keys do
                            // do _value * 10 - (key -'0'), or
                            // do (_value * 8) + (_value * 2) - (key - '0') or
                            // do (_value <<3) + (_value <<1) - (key - '0') and check overflow at each step
                            else
                            {
                                overflow = false;
                                t1 = _value;
                                for(n = 0; n < 3; n++) // << 3 first
                                {
                                    t1 = t1 << 1;
                                    if((t1 & min) == 0)
                                    {
                                        // we went from negative to positive
                                        overflow = true;
                                        break;
                                    }
                                }
                                if(overflow) break;
                                t2 = _value << 1;
                                if((t2 & min) == 0) break;
                                tempval = t1 + t2;
                                if((tempval & min) == 0) break;
                                tempval2 = (key -'0');
                                tempval -= tempval2;
                                if((tempval & min) == 0) break;

                                tempval = status::clipToBitLength(tempval, _bitlength);
                                acceptinput = true; 
                            }

                        }
                        else 
                        {
                            // positive number in input or 0
                            tempval = (_value * 10) + (key - '0'); 
                            tempval = status::clipToBitLength(tempval, _bitlength);
                            if(tempval <= max) acceptinput = true;
                        }
                    }
                    else
                    {
                        // Unsigned integer in decimal
                        tempval = (_value * 10) + (key - '0');
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
                        if(tempval <= max) acceptinput = true;
                        tempval = status::clipToBitLength(tempval, _bitlength);
                    }
                    if(acceptinput) _value = tempval;
                    break;
                default:
                    break;
            }
        }
    }

    if(!acceptinput)
    {
        _valueToString();
        _flashWarning();
    }

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