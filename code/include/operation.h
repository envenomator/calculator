#ifndef OPERATION_H
#define OPERATION_H

#include "displayobject.h"
#include "status.h"

#define DISPLAYSTRINGMAX    32+1

class operation: public displayObject
{
    public:

        enum Method
        {
            None,
            And,
            Or,
            Xor,
            Not,
            SL,
            SR,
            RL,
            RR,
            Modulo,
            Divide,
            Multiply,
            Minus,
            Plus
        };

        operation();
        void set(Method currentmethod);
        uint32_t perform(uint32_t opA, uint32_t opB, uint8_t bitlength, status::Base base, bool sign=false);
        bool inProgress();
        bool getCarry();                // get carry flag for last operation
        bool getOverflow();             // get overflow flag for last operation
        bool error();                   // status of last operation - false == no error
        void clearError();
        void showError();

    protected:
        bool _active;           // active operation in progress
        bool _errorstatus;
        bool _carryflag;        // carry flag status at last operation
        bool _overflowflag;     // overflow flag at last operation
        Method _currentmethod;
        char _displaystring[DISPLAYSTRINGMAX];    // representation of current value as a string
        void _display();
        uint32_t maskToBitLength(uint32_t value, uint8_t bitlength);  // mask to specific bitlength
};

#endif