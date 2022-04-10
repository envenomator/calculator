#include "status.h"
#include "displayvalue.h"
#include <string.h>

status::status(Adafruit_ST7789 *tft)
{
    this->_tft = tft;
}

void status::setArea(area newarea)
{
    _tftarea = newarea;
}

void status::set(displayValue::Base base, uint8_t bitlength, bool sign)
{
    _base = base;
    _bitlength = bitlength;
    _sign = sign;
    display();
}

void status::set(displayValue::Base base)
{
    _base = base;
    display();
}

void status::set(uint8_t bitlength)
{
    _bitlength = bitlength;
    display();
}

void status::set(bool sign)
{
    _sign = sign;
    display();
}

void status::display()
{
    char message[STATUSSTRINGMAXLENGTH];
    char temp[5];

    // clear first
    _tft->fillRect(_tftarea.getTopLeft().getx(),
                   _tftarea.getTopLeft().gety(),
                   _tftarea.width(),
                   _tftarea.height(),
                   ST77XX_BLACK);

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