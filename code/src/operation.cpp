#include "operation.h"
#include "status.h"
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
    _errorstatus = false;
}

bool operation::error()
{
    return _errorstatus;
}

bool operation::inProgress()
{
    return _active;
}

uint32_t operation::perform(uint32_t opA, uint32_t opB, uint8_t bitlength, status::Base base)
{
    uint64_t result = 0;

    _errorstatus = false;   // start out positive

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
            result = maskToBitLength(opA << opB, bitlength);
            break;
        case Method::SR:
            result = maskToBitLength(opA >> opB, bitlength);
            break;
        case Method::RL:
            result = maskToBitLength((opA << opB) | (opA >> (bitlength - opB)),bitlength);
            break;
        case Method::RR:
            result = maskToBitLength((opA >> opB) | (opA << (bitlength - opB)),bitlength);
            break;
        case Method::Modulo:
            result = opA % opB;
            break;
        case Method::Divide:
            if(opB == 0)
            {
                _errorstatus = true;
                strcpy(_displaystring, "Divide by 0");
                return 0; // avoid divide by 0
            }
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
    return (uint32_t)result;
}

uint32_t operation::maskToBitLength(uint32_t value, uint8_t bitlength)
{
    switch(bitlength)
    {
        case 8:
            return value &= 0xFF;
            break;
        case 16:
            return value &= 0xFFFF;
            break;
        case 32:
            return value &= 0xFFFFFFFF;
            break;
    }
    return value;
}

void operation::set(Method currentmethod)
{
    _currentmethod = currentmethod;
    _active = (currentmethod != Method::None);
    strcpy(_displaystring, methodNames[currentmethod]);
    _errorstatus = false;
}

void operation::_display()
{
    _tft->setTextSize(3);
    if(_errorstatus)
        _tft->setTextColor(ST77XX_RED, _bgcolor);
    else
        _tft->setTextColor(_fgcolor,_bgcolor);
    _tft->setCursor(_tftarea.getBottomRight().getx() - (strlen(_displaystring) * 18 - 1),
                _tftarea.getTopLeft().gety());
    _tft->print(_displaystring);
}

void operation::showError()
{
    _display();
}

void operation::clearError()
{
    _errorstatus = false;
    strcpy(_displaystring, "");
    _clear();
}