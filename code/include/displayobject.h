#ifndef DISPLAYOBJECT_H
#define DISPLAYOBJECT_H

#include <Adafruit_ST7789.h>
#include "area.h"

class displayObject
{
        displayObject();
        void init(Adafruit_ST7789 *tft, area newarea, uint16_t bgcolor = ST77XX_BLACK);
        void show();
        void hide();

    protected:
        bool                _show;      // do we show, or keep our area blank
        Adafruit_ST7789 *   _tft;       // tft object to display on
        area                _tftarea;   // our canvas display area
        uint16_t            _bgcolor;

        virtual void _display();        // override by implementor class later
        void _clear();          // just clears the area in this class, can be overridden for specific behaviour
};

#endif