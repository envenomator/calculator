#ifndef OPERATION_H
#define OPERATION_H

#include "displayobject.h"

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
        uint32_t perform(uint32_t opA, uint32_t opB);
        bool inProgress();
        
    protected:
        bool _active;
        Method _currentmethod;
        char _displaystring[DISPLAYSTRINGMAX];    // representation of current value as a string
        void _display();

};

#endif