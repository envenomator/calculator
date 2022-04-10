#ifndef AREA_H
#define AREA_H

#include <stdint.h>
#include "point.h"
class area
{
    public:
        area(uint16_t topleftx, uint16_t toplefty, uint16_t bottomrightx, uint16_t bottomrighty);
        area(point topleft, point bottomright);
        area();

        uint16_t width();
        uint16_t height();

        point getTopLeft();
        point getBottomRight();

    private:
        point topleft, bottomright;

};

#endif