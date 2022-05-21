#ifndef DISPLAYOBJECT_H
#define DISPLAYOBJECT_H

#include <Adafruit_ST7789.h>
#include "area.h"

class displayObject
{
    public:
        displayObject();
        void init(Adafruit_ST7789 *tft, area newarea, uint16_t fgcolor = ST77XX_WHITE, uint16_t altcolor = ST77XX_WHITE, uint16_t bgcolor = ST77XX_BLACK);
        void show();
        void hide();

        void setfgColor(uint16_t color);
        void setaltColor(uint16_t color);
        void setbgColor(uint16_t color);

    protected:
        bool                _show;      // do we show, or keep our area blank
        Adafruit_ST7789 *   _tft;       // tft object to display on
        area                _tftarea;   // our canvas display area
        uint16_t            _bgcolor;   // Background color
        uint16_t            _fgcolor;   // Foreground color
        uint16_t            _altcolor;  // alternative swap color

        virtual void _display();        // override by implementor class later
        virtual void _clear();          // just clears the area in this class, can be overridden for specific behaviour
};

#endif