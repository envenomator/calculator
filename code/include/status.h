#ifndef STATUS_H
#define STATUS_H
#include "area.h"
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include "displayvalue.h"

#define STATUSSTRINGMAXLENGTH 32+1

class status
{
    public:
        status(Adafruit_ST7789 *tft);
        void set(displayValue::Base base, uint8_t bitlength, bool sign);
        void set(displayValue::Base base);
        void set(uint8_t bitlength);
        void set(bool sign);
        void setArea(area newarea);
        void display();

        private:
            Adafruit_ST7789 *_tft;
            area _tftarea;
            displayValue::Base _base;
            uint8_t _bitlength;
            bool _sign;

};

#endif