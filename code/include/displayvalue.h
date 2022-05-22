#ifndef DISPLAYVALUE_H
#define DISPLAYVALUE_H

#include "status.h"
#include "displayobject.h"

#define DISPLAYSTRINGMAX    32+3 // for 0b / 0x etc, two chars extra

#define BINHEADER   "0b"
#define HEXHEADER   "0x"

class displayValue: public displayObject, public status
{
    public:
        displayValue(uint32_t value, status::Base base, uint8_t bitlength, bool sign): status (value, base, bitlength, sign)
        {
            if(_base == status::Dec) strcpy(_displaystring, "0");
            if(_base == status::Hex) strcpy(_displaystring, HEXHEADER"0");
            if(_base == status::Bin) strcpy(_displaystring, BINHEADER"0");

            _currentLength = 0;
            _setDigitLimits();
            _fontsize = 0;  // invalid default, will be set at first _display()
            _fontwidth = 0; // invalid default
        }

        void setValue(uint32_t);
        void setBase(status::Base base);
        void setBitLength(uint8_t bitlength);
        void setSign(bool sign);

    protected:
        uint8_t _currentLength;     // number of digits currently displaying
        uint8_t _currentMaxLength;  // maximum number of digits to display
        char _displaystring[DISPLAYSTRINGMAX];    // representation of current value as a string

        void _setDigitLimits();     // sets the limit on the amount of digits, based on the base
        void _display();
        void _valueToString();
        void _setBitLength(uint8_t bitlength);
        void display32bit();

        uint8_t _fontsize;
        uint8_t _fontwidth;

    private:
        const char *bit_rep[16] = {
            [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
            [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
            [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
            [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
        };
};

#endif