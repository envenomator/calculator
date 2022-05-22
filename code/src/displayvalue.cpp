#include "displayvalue.h"
#include <string.h>
#include <inttypes.h>

void displayValue::setValue(uint32_t value)
{
    status::setValue(value);
    _valueToString();
    if(_show)
    {
        // clear screen
        _clear();
        // update current value
        _display();
    }
}

void displayValue::_valueToString()
{
    char temp[DISPLAYSTRINGMAX];
    char *tmp;
    uint32_t v;

    // prepare display string
    switch(_base)
    {
        case Bin:
            strcpy(_displaystring, BINHEADER);

            // convert 32-bit integer to 32 character string with 0/1s
            v = _value;
            tmp = temp + 31;

            for(uint8_t i = 0; i < 32; i++)
            {
                *tmp = (v & 0x1) + 48;
                tmp--;
                v = v >> 1;
            }
            temp[32] = 0;

            // find first non-zero character
            tmp = temp;
            while((*tmp == '0') && (*(tmp+1) != 0)) tmp++;

            strcat(_displaystring, tmp);

            _currentLength = strlen(tmp);
            if(_currentLength == 1 && temp[0] == '0') _currentLength = 0;
            break;
        case Hex:
            snprintf(temp, DISPLAYSTRINGMAX, "%" PRIX32 "", _value);
            strcpy(_displaystring, HEXHEADER);
            strcat(_displaystring, temp);
            _currentLength = strlen(temp);
            if(_currentLength == 1 && temp[0] == '0') _currentLength = 0;
            break;
        case Dec:
            if(_sign)
            {
                // Create signed strings per bitlength type
                switch(_bitlength)
                {
                    case 8:
                        snprintf(temp, DISPLAYSTRINGMAX, "%" PRIi8 "", (int8_t)_value);
                        break;
                    case 16:
                        snprintf(temp, DISPLAYSTRINGMAX, "%" PRId16 "", (int16_t)_value);
                        break;
                    case 32:
                        snprintf(temp, DISPLAYSTRINGMAX, "%" PRId32 "", (int32_t)_value);
                        break;
                }
            } 
            else
                // Unsigned - just create the string at maximum 32bit length
                snprintf(temp, DISPLAYSTRINGMAX, "%" PRIu32 "", _value);
            strcpy(_displaystring, temp);
            _currentLength = strlen(temp);
            // zero result? - length is zero digits
            if(_currentLength == 1 && temp[0] == '0') _currentLength = 0;
            // decimal with 2s complement and negative number? - subtract one digit for '-'
            if(_base == status::Dec && _sign && status::isNegative(_value,_bitlength)) _currentLength--;
            break;
        default:
            break;
    }

}

void displayValue::setBitLength(uint8_t bitlength)
{
    status::setBitLength(bitlength);
    _setDigitLimits();
    _valueToString();
    if(_show) 
    {
        _clear();
        _display();
    }
}

void displayValue::_setDigitLimits()
{
    switch(_base)
    {
        case Dec:
            switch(_bitlength)
            {
                case 8:
                    _currentMaxLength = 3;
                    break;
                case 16:
                    _currentMaxLength = 5;
                    break;
                case 32:
                    _currentMaxLength = 10;
                    break;
                default:
                    _currentMaxLength = 3;
                    break;
            }
            break;
        case Bin:
            _currentMaxLength = _bitlength;
            break;
        case Hex:
            _currentMaxLength = _bitlength / 4;
            break;
    }
}

void displayValue::setBase(status::Base base)
{
    status::setBase(base);
    _setDigitLimits();
    _valueToString();
    if(_show)
    {
        _clear();
        _display();
    }
}

void displayValue::setSign(bool sign)
{
    status::setSign(sign);
    if(_base == status::Dec && _show) // only change display if current base is Decimal
    {
        _clear();
        _valueToString();
        _display();
    }
}

void displayValue::_display()
{
    uint8_t fontsize;
    uint8_t length;
    uint8_t blocks,startchars,i;

    char buffer[5];
    char *ptr;

    if(_base == status::Bin)
    {
        length = strlen(_displaystring);
        if(length < 18)
            fontsize = 3;
        else
        {
            if(length < 27)
                fontsize = 2;
            else
                fontsize = 1;
        }
    }
    else fontsize = 3; // Hex and Dec -- large font

    //clear the are when the fontsize changes
    if(_fontsize != fontsize)
    {
        _fontsize = fontsize;
        _fontwidth = (_fontsize * 5) + _fontsize; // calculate new font width
        _clear();
    }

    // setup start position and fontsizes
    _tft->setTextSize(_fontsize);
    _tft->setTextColor(_fgcolor,_bgcolor);
    _tft->setCursor(_tftarea.getBottomRight().getx() - (strlen(_displaystring) * _fontwidth - 1),
                    _tftarea.getTopLeft().gety());

    // handle each base differently on display
    switch(_base)
    {
        case Dec:
            _tft->print(_displaystring);
            break;
        case Bin:
            _clear();   // needed for color block changes on most entries
            _tft->print(BINHEADER);
            if(_currentLength == 0) _tft->print("0");

            blocks = _currentLength >> 2;
            startchars = _currentLength % 4;
            ptr = _displaystring + 2;

            buffer[4] = 0;
            strncpy(buffer, ptr, startchars);
            ptr += startchars;
            buffer[startchars] = 0;
            if(blocks)
                _tft->setTextColor(blocks&1?_altcolor:_fgcolor);
            else
                _tft->setTextColor(_fgcolor);
            _tft->print(buffer);

            for(i = 0; i < blocks; i++)
            {

                if(blocks & 1)
                    _tft->setTextColor(i&1?_altcolor:_fgcolor);
                else
                    _tft->setTextColor(i&1?_fgcolor:_altcolor);
                strncpy(buffer, ptr, 4);
                _tft->print(buffer);
                ptr += 4;
            }
            break;
        case Hex:
            _tft->print(_displaystring);
            break;
    }
}

void displayValue::display32bit()
{
  char buffer[17];
  uint8_t byte;
  uint16_t word;
  
  _tft->setCursor(0,10);
  _tft->setTextSize(1);
  _tft->setTextColor(_fgcolor);
  _tft->print("MSW");
  _tft->setCursor(0,40);
  _tft->setTextSize(1);
  _tft->setTextColor(_fgcolor);
  _tft->print("LSW");

  _tft->setTextSize(3);
  
  word = _value >> 16;
  byte = word >> 8;
  _tft->setCursor(32,0);
  snprintf(buffer,17,"%s",bit_rep[byte >> 4]);
  _tft->setTextColor(ST77XX_WHITE);
  _tft->print(buffer);
  snprintf(buffer,17,"%s",bit_rep[byte & 0x0F]);
  _tft->setTextColor(ST77XX_BLUE);
  _tft->print(buffer);

  byte = word & 0xFF;
  snprintf(buffer,17,"%s",bit_rep[byte >> 4]);
  _tft->setTextColor(ST77XX_WHITE);
  _tft->print(buffer);
  snprintf(buffer,17,"%s",bit_rep[byte & 0x0F]);
  _tft->setTextColor(ST77XX_BLUE);
  _tft->print(buffer);

  word = _value & 0xFFFF;
  byte = word >> 8;
  _tft->setCursor(32,30);
  snprintf(buffer,17,"%s",bit_rep[byte >> 4]);
  _tft->setTextColor(ST77XX_BLUE);
  _tft->print(buffer);
  snprintf(buffer,17,"%s",bit_rep[byte & 0x0F]);
  _tft->setTextColor(ST77XX_WHITE);
  _tft->print(buffer);

  byte = word & 0xFF;
  snprintf(buffer,17,"%s",bit_rep[byte >> 4]);
  _tft->setTextColor(ST77XX_BLUE);
  _tft->print(buffer);
  snprintf(buffer,17,"%s",bit_rep[byte & 0x0F]);
  _tft->setTextColor(ST77XX_WHITE);
  _tft->print(buffer);
}

