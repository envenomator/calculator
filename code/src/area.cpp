#include "area.h"
#include "point.h"

// Constructor
area::area(point topleft, point bottomright)
{
    this->topleft = topleft;
    this->bottomright = bottomright;
}

area::area(uint16_t topleftx, uint16_t toplefty, uint16_t bottomrightx, uint16_t bottomrighty)
{
    this->topleft = point(topleftx, toplefty);
    this->bottomright = point(bottomrightx, bottomrighty);
}

area::area()
{
    this->topleft = point(0,0);
    this->bottomright = point(0,0);
}

uint16_t area::width()
{
    return this->topleft.width(this->bottomright);
}

uint16_t area::height()
{
    return this->topleft.height(this->bottomright);
}

point area::getTopLeft()
{
    return topleft;
}

point area::getBottomRight()
{
    return bottomright;
}
