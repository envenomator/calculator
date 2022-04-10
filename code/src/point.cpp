#include "point.h"
#include <stdlib.h>

point::point(uint16_t x, uint16_t y)
{
    this->x = x;
    this->y = y;
}

point::point()
{
    this->x = 0;
    this->y = 0;
}

uint16_t point::getx()
{
    return x;
}

uint16_t point::gety()
{
    return y;
}

uint16_t point::width(point to)
{
    return abs(x - to.getx()) + 1;
}

uint16_t point::height(point to)
{
    return abs(y - to.gety()) + 1;
}

