#include "status.h"
#include "displayvalue.h"
#include <string.h>

status::status(displayValue::Base base, uint8_t bitlength, bool sign)
{
    set(base,bitlength,sign);
}

void status::set(displayValue::Base base, uint8_t bitlength, bool sign)
{
    _base = base;
    _bitlength = bitlength;
    _sign = sign;
    if(_show) _display();
}

void status::set(displayValue::Base base)
{
    _base = base;
    if(_show) _display();
}

void status::set(uint8_t bitlength)
{
    _bitlength = bitlength;
    if(_show) _display();
}

void status::set(bool sign)
{
    _sign = sign;
    if(_show) _display();
}

void status::_display()
{
    char message[STATUSSTRINGMAXLENGTH];
    char temp[5];

    // clear first
    _clear();
    // create message
    // Base type
    switch(_base)
    {
        case displayValue::Hex:
            sprintf(message,"HEX ");
            break;
        case displayValue::Bin:
            sprintf(message,"BIN ");
            break;
        case displayValue::Dec:
            sprintf(message,"DEC ");
            break;
    }
    // bit length
    itoa(_bitlength, temp, 10);
    strcat(message, temp);
    // Sign
    if(_sign)
        strcat(message, "bit");
    else
        strcat(message, "bit");

    // Add sign if needed
    if(_base == displayValue::Dec)
    {
        if(_sign)
            strcat(message, " signed");
        else
            strcat(message, " unsigned");
    }
    // Display message
    _tft->setTextSize(2);
    _tft->setTextColor(ST77XX_GREEN,ST77XX_BLACK);
    _tft->setCursor(_tftarea.getTopLeft().getx(), _tftarea.getTopLeft().gety());
    _tft->print(message);
}