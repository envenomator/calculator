#include "displayobject.h"

displayObject::displayObject()
{
    _show = false;
    _tft = NULL;
}

void displayObject::init(Adafruit_ST7789 *tft, area newarea, uint16_t fgcolor, uint16_t bgcolor)
{
    _tft = tft;
    _tftarea = newarea;
    _fgcolor = fgcolor;
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

void displayObject::_display()
{

}

void displayObject::setbgColor(uint16_t color)
{
    _bgcolor = color;
}

void displayObject::setfgColor(uint16_t color)
{
    _fgcolor = color;
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