#ifndef POINT_H
#define POINT_H

#include <stdint.h>

class point
{
    public:
        point(uint16_t x, uint16_t y);
        point();

        uint16_t width(point to);
        uint16_t height(point to);

        uint16_t getx();
        uint16_t gety();

    private:
        uint16_t x,y;


};

#endif