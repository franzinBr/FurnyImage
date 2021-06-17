#ifndef POINT_HPP
#define POINT_HPP

#include <stdlib.h>

class Point
{
private:
    /* data */
    
public:
    int x,y;

    Point();
    Point(int x, int y);

    static int* lengthXY(Point p1, Point p2);
    
};

#endif // POINT_HPP

