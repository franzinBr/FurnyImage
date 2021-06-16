#ifndef RGB_HPP
#define RGB_HPP

#include <stdint.h>

class Rgb
{
private:
    /* data */
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Rgb();
    Rgb(uint8_t unique);
    Rgb(uint8_t r, uint8_t g, uint8_t b);
    Rgb(int unique);
    Rgb(int r, int g, int b);
    ~Rgb();
};

#endif // RGB_HPP