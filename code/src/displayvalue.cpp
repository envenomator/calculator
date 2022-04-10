#include "displayvalue.h"
#include <string.h>
#include <inttypes.h>

// Constructor
displayValue::displayValue(uint32_t value, Base base, uint8_t bitlength)
{
    _currentValue = value;
    _currentBase = base;
    _setBitLength(bitlength);
    _currentLength = 0;
    strcpy(_displaystring,"0");
    if(_currentBase == displayValue::Dec) strcpy(_displaystring, "0");
    if(_currentBase == displayValue::Hex) strcpy(_displaystring, "0x0");
    if(_currentBase == displayValue::Bin) strcpy(_displaystring, "0b0");
}

uint32_t displayValue::get()
{
    return _currentValue;
}

void displayValue::set(uint32_t value)
{
    char temp[DISPLAYSTRINGMAX];

    _currentValue = value;
    // prepare display string
    switch(_currentBase)
    {
        case Bin:
            itoa(_currentValue, temp, 2);
            strcpy(_displaystring, "0b");
            strcat(_displaystring, temp);
            _currentLength = strlen(temp);
            if(_currentLength == 1 && temp[0] == '0') _currentLength = 0;
            break;
        case Hex:
            itoa(_currentValue, temp, 16);
            strupr(temp);
            strcpy(_displaystring, "0x");
            strcat(_displaystring, temp);
            _currentLength = strlen(temp);
            if(_currentLength == 1 && temp[0] == '0') _currentLength = 0;
            break;
        case Dec:
            snprintf(temp, DISPLAYSTRINGMAX, "%" PRIu32 "", _currentValue);
            strcpy(_displaystring, temp);
            _currentLength = strlen(temp);
            if(_currentLength == 1 && temp[0] == '0') _currentLength = 0;
            break;
        default:
            break;
    }
    if(_show)
    {
        // clear screen
        _clear();
        // update current value
        _display();
    }
}

void displayValue::setBitLength(uint8_t bitlength)
{
    _setBitLength(bitlength);
    if(_show) _display();
}

void displayValue::_setBitLength(uint8_t bitlength)
{
    _currentBitLength = bitlength;
    switch(_currentBase)
    {
        case Dec:
            switch(bitlength)
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
            _currentMaxLength = bitlength;
            break;
        case Hex:
            _currentMaxLength = bitlength / 4;
            break;
    }
}

void displayValue::setBase(Base newbase)
{
    _currentBase = newbase;
    _setBitLength(_currentBitLength);
    set(_currentValue);

    if(_show) _display();
}

void displayValue::_display()
{
    _tft->setTextSize(3);
    _tft->setTextColor(ST77XX_BLUE,ST77XX_BLACK);
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
  _tft->setTextColor(ST77XX_WHITE);
  _tft->print("MSW");
  _tft->setCursor(0,40);
  _tft->setTextSize(1);
  _tft->setTextColor(ST77XX_BLUE);
  _tft->print("LSW");

  _tft->setTextSize(3);
  
  word = _currentValue >> 16;
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

  word = _currentValue & 0xFFFF;
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

