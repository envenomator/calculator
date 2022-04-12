#include "statusflags.h"
#include <string.h>

statusFlags::statusFlags()
{
    clearFlags();
}

void statusFlags::setFlags(bool carry, bool negative, bool overflow)
{
    _cflag = carry;
    _nflag = negative;
    _oflag = overflow;
    if(_show) _display();
}

void statusFlags::clearFlags()
{
    _cflag = false;
    _nflag = false;
    _oflag = false;
    if(_show) _display();    
}

void statusFlags::_display()
{
    char message[STATUSSTRINGMAXLENGTH];
    // clear first
    _clear();
    // create message
    if(_cflag || _nflag || _oflag)
    {
        _tft->setTextSize(2);
        _tft->setTextColor(_fgcolor,_bgcolor);
        strcpy(message,"[");
        if(_cflag) strcat(message,"C");
        if(_nflag) strcat(message,"N");
        if(_oflag) strcat(message,"O");
        strcat(message,"]");
        _tft->setCursor(_tftarea.getBottomRight().getx() - (strlen(message) * 12 - 1),
                        _tftarea.getTopLeft().gety());
        _tft->print(message);
    }
}