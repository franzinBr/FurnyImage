#ifndef IMAGE_HPP
#define IMAGE_HPP
#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)

#include <stdint.h>
#include <cstdint>
#include <cstddef>
#include <math.h>
#include "Rgb.hpp"
#include "Point.hpp"
#include "ImageMath.hpp"


enum ImageType { PNG, JPG, BMP};
enum TreshType { BINARY, BINARY_INV, TRUNC, TOZERO, TOZERO_INV};
enum ConvPadType {ZERO, NEAR_NEIGHBOR, CYCLIC};

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
    Image& crop(Point p1,  Point p2 );
    Image& thresholding(uint8_t treshValue, TreshType type);

    // Convolution 
    Image& conv(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc, ConvPadType convType = NEAR_NEIGHBOR);
    Image& convFourier(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc, ConvPadType convType = NEAR_NEIGHBOR);
};





#endif // IMAGE_HPP