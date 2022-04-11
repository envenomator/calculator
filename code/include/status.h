#ifndef STATUS_H
#define STATUS_H
#include <stdint.h>

class status
{
    public:
       enum Base
        {
            Dec,
            Hex,
            Bin
        };

        status(uint32_t value, Base base, uint8_t bitlength, bool sign);
        uint32_t getValue();
        void setValue(uint32_t value);
        void setBase(Base base);
        void setBitLength(uint8_t bitlength);
        uint8_t getBitLength();
        void setSign(bool sign);
        bool isNegative();

    protected:
        uint32_t _value;
        Base _base;
        uint8_t _bitlength;
        bool _sign;

        void _clipToBitLength();    // clip uint32_t to chosen bitlength mask
};

#endif