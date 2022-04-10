#ifndef DISPLAYVALUE_H
#define DISPLAYVALUE_H

#include <stdint.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include "point.h"
#include "area.h"

#define DISPLAYSTRINGMAX    32+1
class displayValue
{
    public:
        enum Base
        {
            Dec,
            Hex,
            Bin
        };

        displayValue(uint32_t value, Base base, uint8_t bitlength, Adafruit_ST7789 *tft);
        uint32_t get();
        void set(uint32_t value);
        void setBitLength(uint8_t bitlength);
        void setBase(Base newbase);
        void setArea(area newarea);
        void hide();
        void display();

    protected:
        uint32_t _currentValue;     
        uint8_t _currentBitLength;  // current bit length (8/16/32)
        uint8_t _currentLength;     // number of digits currently displaying
        uint8_t _currentMaxLength;  // maximum number of digits to display
        bool _display;              // do we display, or keep our area blank
        Base _currentBase;          // current number base
        Adafruit_ST7789 *_tft;
        area _tftarea;
        char _displaystring[DISPLAYSTRINGMAX];    // representation of current value as a string

        void _setBitLength(uint8_t bitlength);
        void display32bit();
        void _clear();

        const char *bit_rep[16] = {
            [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
            [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
            [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
            [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
        };
};

#endif