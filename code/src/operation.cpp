#include "operation.h"

#define MAXMETHODNAMELENGTH 32
char methodNames[][MAXMETHODNAMELENGTH]=
{
    "",
    "AND",
    "OR",
    "XOR",
    "NOT",
    "Shift LEFT",
    "Shift RIGHT",
    "Rotate LEFT",
    "Rotate RIGHT",
    "%",
    "/",
    "*",
    "-",
    "+" 
};

operation::operation()
{
    strcpy(_displaystring, "");
    _active = false;
    _currentmethod = Method::None;
}

bool operation::active()
{
    return _active;
}

void operation::set(Method currentmethod)
{
    _currentmethod = currentmethod;
    _active = (currentmethod != Method::None);
    strcpy(_displaystring, methodNames[currentmethod]);
}

void operation::_display()
{
    _tft->setTextSize(3);
    _tft->setTextColor(_fgcolor,_bgcolor);
    _tft->setCursor(_tftarea.getBottomRight().getx() - (strlen(_displaystring) * 18 - 1),
                _tftarea.getTopLeft().gety());
    _tft->print(_displaystring);
}