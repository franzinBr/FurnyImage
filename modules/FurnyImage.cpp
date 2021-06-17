#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "FurnyImage.hpp"
#include "../external_dependences/stb_image.h"
#include "../external_dependences/stb_image_write.h"

Image::Image() {}

Image::Image(const char *filename)
{
    bool isOpen = open(filename);
    if(!isOpen){printf("Failed to open %s\n", filename);}
    if(isOpen)
    {
        size = w*h*channels;
    }
}

Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels)
{
    size = w*h*channels;
    data = new uint8_t[size];
}

Image::Image(int w, int h, Rgb &rgb) : Image(w, h, 3)
{
    for(size_t i = 0; i < size; i += channels)
    {
        data[i] = rgb.r;
        data[i+1] = rgb.g;
        data[i+2] = rgb.b;
    }
}
Image::Image(int w, int h, Rgb &&rgb) : Image(w, h, rgb) {}

Image::Image(const Image &img) : Image(img.w, img.h, img.channels)
{
    memcpy(data, img.data, size);
}

Image::~Image()
{
    stbi_image_free(data);
}

Image& Image::operator = (Image &img)
{
    if(this != (&img))
    {
        if(data != NULL) stbi_image_free(data);
        w = img.w;
        h = img.h;
        channels = img.channels;
        size = img.size;
        data = new uint8_t[size];
        memcpy(data, img.data, size);
    }
    return *this;
}

Image Image::operator + (Image &img)
{
    if(w == img.w && h == img.h && channels == img.channels)
    {
        Image tmp(w, h, channels);
        for(size_t i = 0; i < size; i += channels)
        {
            for(size_t j = 0; j < channels; j++)
            {
                tmp.data[i+j] = sqrt((data[i+j]*data[i+j] + img.data[i+j]*img.data[i+j]) / 2);  
                //BYTE_BOUND(data[i+j] + img.data[i+j]);
            }
        }
        return tmp;
    }
    printf("Operation valid only for images with the same resolution\n");
    Image empty(w, h, channels);
    return empty;
    
}

ImageType Image::getType(const char *filename)
{
    const char *extension = strrchr(filename, '.');
    if(extension != nullptr)
    {
        if(strcmp(extension, ".png") == 0) return PNG;
		else if(strcmp(extension, ".jpg") == 0) return JPG;
		else if(strcmp(extension, ".bmp") == 0) return BMP;
    }
    printf("the format provided is invalid. The file will be saved using the default type.\n");
    return defaultType;
}

bool Image::open(const char *filename)
{
    data = stbi_load(filename, &w, &h, &channels, 0);
    return data != NULL;
}

bool Image::save(const char *filename)
{
    ImageType type = getType(filename);
    int success;
    switch(type)
    {
        case PNG:
            success = stbi_write_png(filename, w, h, channels, data, w*channels);
        break;
        case JPG:
            success = stbi_write_jpg(filename, w, h, channels, data, 100);
        break;
        case BMP:
            success = stbi_write_bmp(filename, w, h, channels, data);
        break;
        default:
            success = stbi_write_png(filename, w, h, channels, data, w*channels);
        break;
    }
    return success != 0;

}