#include "Point.hpp"

Point::Point() {}
Point::Point(int x, int y): x(x), y(y) {}


int* Point::lengthXY(Point p1, Point p2)
{
    int *len = new int[2]; //
    len[0] = abs(p2.x - p1.x);
    len[1] = abs(p2.y - p1.y);
    return len;
}