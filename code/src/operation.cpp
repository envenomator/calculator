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
    _carryflag = false;
    _overflowflag = false;
}

bool operation::error()
{
    return _errorstatus;
}

bool operation::inProgress()
{
    return _active;
}

bool operation::getCarry()
{
    return _carryflag;
}

bool operation::getOverflow()
{
    return _overflowflag;
}

uint32_t operation::perform(uint32_t opA, uint32_t opB, uint8_t bitlength, status::Base base, bool sign)
{
    uint64_t result = 0;
    bool negA, negB, negResult;

    _errorstatus = false;   // start out positive
    _carryflag = false;     // disregard any old carry flag
    _overflowflag = false;  // disregard any old overflow flag

    negA = status::isNegative(opA,bitlength);
    negB = status::isNegative(opB,bitlength);

    switch(_currentmethod)
    {
        case Method::None:
            result = opA;
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
            if(opB == 0)
            {
                _errorstatus = true;
                strcpy(_displaystring, "Divide by 0");
                return 0; // avoid divide by 0
            }
            else
            {
                if(sign)
                {
                    switch(bitlength)
                    {
                        case 8:
                            result = (int8_t)((int8_t)opA % (int8_t)opB);
                            break;
                        case 16:
                            result = (int16_t)((int16_t)opA % (int16_t)opB);
                            break;
                        case 32:
                            result = (int32_t)((int32_t)opA % (int32_t)opB);
                            break;
                        default:
                            result = 0;
                    }
                }
                else result = opA % opB;
            }
            break;
        case Method::Divide:
            if(opB == 0)
            {
                _errorstatus = true;
                strcpy(_displaystring, "Divide by 0");
                return 0; // avoid divide by 0
            }
            else
            {
                if(sign)
                {
                    switch(bitlength)
                    {
                        case 8:
                            result = (int8_t)((int8_t)opA / (int8_t)opB);
                            break;
                        case 16:
                            result = (int16_t)((int16_t)opA / (int16_t)opB);
                            break;
                        case 32:
                            result = (int32_t)((int32_t)opA / (int32_t)opB);
                            break;
                        default:
                            result = 0;
                    }
                }
                else result = opA / opB;
            }
            break;
        case Method::Multiply:
            result = opA * opB;
            break;
        case Method::Minus:
            result = opA - opB;
            // Check overflow flag
            negResult = status::isNegative(result, bitlength);
            if(!negA && negB && negResult) _overflowflag = true;    // + min - results in -
            if(negA && !negB && !negResult) _overflowflag = true;   // - min + results in +
            break;
        case Method::Plus:
            result = opA + opB;
            // Check overflow flag
            negResult = status::isNegative(result, bitlength);
            if(negA && negB && !negResult) _overflowflag = true;    // + plus + results in -
            if(!negA && !negB && negResult) _overflowflag = true;   // - plus - results in +
            break;
        default:
            break; 
    }
    _carryflag = status::hasCarry(result, bitlength);
    _currentmethod = operation::None;   // clear operation for next, even when 'empty'
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