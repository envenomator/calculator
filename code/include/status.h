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
        Base getBase();
        void setBitLength(uint8_t bitlength);
        uint8_t getBitLength();
        void setSign(bool sign);
        bool getSign();

        static bool isNegative(uint32_t value, uint8_t bitlength);
        static bool hasCarry(uint64_t value, uint8_t bitlength);

        static uint32_t clipToBitLength(uint32_t val, uint8_t bitlength);  //clip any uint32_t to length;

    protected:
        uint32_t _value;
        Base _base;
        uint8_t _bitlength;
        bool _sign;                 // display sign in Dec mode / 2s complement only

        void _clipToBitLength();    // clip _value with uint32_t to chosen bitlength mask


};

#endif