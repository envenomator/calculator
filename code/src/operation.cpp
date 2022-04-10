#include "operation.h"
#include <stdint.h>

#define MAXMETHODNAMELENGTH 32
char methodNames[][MAXMETHODNAMELENGTH]=
{
    "",
    "AND",
    "OR",
    "XOR",
    "NOT",
    "<< shift bit(s)",
    "shift bit(s) >>",
    "<< rotate bit(s) <",
    "> rotate bit(s) >>",
    "%",
    "/",
    "X",
    "-",
    "+" 
};

operation::operation()
{
    strcpy(_displaystring, "");
    _active = false;
    _currentmethod = Method::None;
}

bool operation::inProgress()
{
    return _active;
}

uint32_t operation::perform(uint32_t opA, uint32_t opB, uint8_t bitlength)
{
    uint32_t result = 0;

    switch(_currentmethod)
    {
        case Method::None:
            break;
        case Method::And:
            result = opA & opB;
            break;
        case Method::Or:
            result = opA | opB;
            break;
        case Method::Xor:
            result = opA ^ opB;
            break;
        case Method::Not:
            result = ~opA;
            break;
        case Method::SL:
            result = opA << opB;
            break;
        case Method::SR:
            result = opA >> opB;
            break;
        case Method::RL:
            result = (opA << opB) | (opA >> (bitlength - opB));
            break;
        case Method::RR:
            result = (opA >> opB) | (opA << (bitlength - opB));
            break;
        case Method::Modulo:
            result = opA % opB;
            break;
        case Method::Divide:
            if(opB == 0)
                result = 0; // avoid divide by 0
            else
                result = opA / opB;
            break;
        case Method::Multiply:
            result = opA * opB;
            break;
        case Method::Minus:
            result = opA - opB;
            break;
        case Method::Plus:
            result = opA + opB;
            break;
        default:
            break; 
    }

    return result;
}

void operation::set(Method currentmethod)
{
    _currentmethod = currentmethod;
    _active = (currentmethod != Method::None);
    strcpy(_displaystring, methodNames[currentmethod]);
}

void operation::_display()
{
    _tft->setTextSize(3);
    _tft->setTextColor(_fgcolor,_bgcolor);
    _tft->setCursor(_tftarea.getBottomRight().getx() - (strlen(_displaystring) * 18 - 1),
                _tftarea.getTopLeft().gety());
    _tft->print(_displaystring);
}