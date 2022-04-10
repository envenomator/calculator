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
        void setSign(bool sign);

    protected:
        uint32_t _value;
        Base _base;
        uint8_t _bitlength;
        bool _sign;
};

#endif