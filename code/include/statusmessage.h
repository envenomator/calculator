#ifndef STATUSMESSAGE_H
#define STATUSMESSAGE_H
#include "status.h"
#include "displayobject.h"

#define STATUSSTRINGMAXLENGTH 32+1

class statusMessage: public displayObject, public status
{
    public:
        statusMessage(uint32_t value, status::Base base, uint8_t bitlength, bool sign): status(value,base,bitlength,sign) {}
        void setBase(Base base);
        void setBitLength(uint8_t bitlength);
        void setSign(bool sign);

    private:
        void _display();

};

#endif