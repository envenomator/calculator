#include "statusmessage.h"
#include "status.h"
#include <string.h>

void statusMessage::setBase(status::Base base)
{
    status::setBase(base);
    if(_show) _display();
}

void statusMessage::setBitLength(uint8_t bitlength)
{
    status::setBitLength(bitlength);
    if(_show) _display();
}

void statusMessage::setSign(bool sign)
{
    status::setSign(sign);
    if(_show) _display();
}

void statusMessage::_display()
{
    char message[STATUSSTRINGMAXLENGTH];
    char temp[5];

    // clear first
    _clear();
    // create message
    // Base type
    switch(_base)
    {
        case status::Hex:
            sprintf(message,"HEX ");
            break;
        case status::Bin:
            sprintf(message,"BIN ");
            break;
        case status::Dec:
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
    if(_base == status::Dec)
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