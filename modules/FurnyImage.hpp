#ifndef IMAGE_HPP
#define IMAGE_HPP
#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)

#include <stdint.h>
#include <cstdint>
#include <cstddef>
#include <math.h>
#include "Rgb.hpp"
#include "Point.hpp"


enum ImageType { PNG, JPG, BMP};

class Image
{
private:
    /* data */
public:
    uint8_t *data = NULL;
    size_t size = 0;
    int w, h, channels;
    ImageType defaultType = PNG; 

    // Constructors&Destructors
    Image();
    Image(const char *filename);
    Image(int w, int h, int channels);
    Image(int w, int h, Rgb &rgb);
    Image(int w, int h, Rgb &&rgb);
    Image(const Image &img);
    ~Image();

    // IO files
    ImageType getType(const char *filename);

    bool open(const char *filename);
    bool save(const char *filename);

    // Operators
    Image& operator = (Image &img);
    Image operator + (Image &img);

    // ImageProc
    Image& flip(uint8_t direction = 0);
    Image& overlap(const Image& img, Point &&point);
    Image& overlap(const Image& img, Point &point);
    Image& toGray();
};





#endif // IMAGE_HPP