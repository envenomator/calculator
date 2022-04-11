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
    // prepare display string
    switch(_base)
    {
        case Bin:
            itoa(_value, temp, 2);
            strcpy(_displaystring, "0b");
            strcat(_displaystring, temp);
            _currentLength = strlen(temp);
            if(_currentLength == 1 && temp[0] == '0') _currentLength = 0;
            break;
        case Hex:
            snprintf(temp, DISPLAYSTRINGMAX, "%" PRIX32 "", _value);
            strcpy(_displaystring, "0x");
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
                        snprintf(temp, DISPLAYSTRINGMAX, "%" PRIi8 "", (uint8_t)_value);
                        break;
                    case 16:
                        snprintf(temp, DISPLAYSTRINGMAX, "%" PRId16 "", (uint16_t)_value);
                        break;
                    case 32:
                        snprintf(temp, DISPLAYSTRINGMAX, "%" PRId32 "", _value);
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
    _tft->setTextSize(3);
    _tft->setTextColor(_fgcolor,_bgcolor);
    _tft->setCursor(_tftarea.getBottomRight().getx() - (strlen(_displaystring) * 18 - 1),
                    _tftarea.getTopLeft().gety());
    _tft->print(_displaystring);
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

