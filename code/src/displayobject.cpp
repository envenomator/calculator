#include "displayobject.h"

displayObject::displayObject()
{
    _show = false;
    _tft = NULL;
}

void displayObject::init(Adafruit_ST7789 *tft, area newarea, uint16_t bgcolor)
{
    _tft = tft;
    _tftarea = newarea;
    _bgcolor = bgcolor;
}

void displayObject::_clear()
{
    _tft->fillRect(_tftarea.getTopLeft().getx(),
                   _tftarea.getTopLeft().gety(),
                   _tftarea.width(),
                   _tftarea.height(),
                   _bgcolor);
}

void displayObject::show()
{
    _show = true;
    _display();
}

void displayObject::hide()
{
    _show = false;
    _clear();
}