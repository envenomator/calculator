#ifndef STATUSFLAGS_H
#define STATUSFLAGS_H
#include "status.h"
#include "displayobject.h"

#define STATUSSTRINGMAXLENGTH 32+1

class statusFlags: public displayObject
{
    public:
        statusFlags();
        void setFlags(bool carry, bool negative, bool overflow);
        void clearFlags();

    private:
        void _display();
        bool _nflag;
        bool _oflag;
        bool _cflag;

};

#endif